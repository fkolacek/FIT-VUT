#include "qircserver.h"

QIrcServer::QIrcServer(QObject *parent): QObject(parent){
    this->logger = NULL;
    this->config = NULL;
    this->server = NULL;
    this->serverTimer = NULL;
}

QIrcServer::~QIrcServer(){
    if(this->serverTimer)
        this->serverTimer->stop();

    delete this->serverTimer;
    delete this->server;
    delete this->users;
    delete this->config;
    delete this->logger;
}

void QIrcServer::run(){
    try{
        //Vytvori instanci tridy logger a nastavi potrebne signaly
        this->initLogger();

        //Pokusi se nacist konfiguracni soubor
        this->initConfig();

        //Nacte databazi uzivatelu
        this->initUsers();

        //Nastavi parametry serveru a spusti jej
        this->initServer();

        //Spusti server timer
        this->initTimer();

    }
    catch(QIrcException* e){
        std::cerr << "[!] " << e->getMessage().toStdString() << std::endl;
        delete e;
    }
    catch(...){
        std::cerr << "[!] Cautched unexpected exception" << std::endl;
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *  Private methods
 * ----------------------------------------------------------------- */

void QIrcServer::initLogger(){
    this->logger = new QIrcLogger("server.log", true, this);

    connect(this, SIGNAL(sendMessage(QString)), this->logger, SLOT(receiveMessage(QString)));

    emit sendMessage("[*] Initializing server");
}

void QIrcServer::initConfig(){
    this->config = new QIrcIniFile(this);

    emit sendMessage("[*] Loading config..");

    this->config->loadFromFile("config.ini");

    this->validateConfig();

    emit sendMessage("[*] Config was successfully loaded");
}

void QIrcServer::initUsers(){
    QString userdb = this->config->getValue("Server", "userdb");

    this->users = new QIrcUserDB(userdb);

    emit sendMessage("[*] Loading users from " + userdb);

    this->users->load();

    emit sendMessage("[*] Successfully loaded " + QString::number(this->users->count()) + " users");
}

void QIrcServer::initServer(){
    this->server = new QTcpServer(this);

    connect(this->server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));

    if(this->config->getValue("Server", "listen") == "any")
        this->serverAddress = QHostAddress(QHostAddress::Any);
    else
        this->serverAddress = QHostAddress(this->config->getValue("Server", "listen"));

    this->serverPort = this->config->getValue("Server", "port").toInt();

    this->server->setMaxPendingConnections(this->config->getValue("Server", "connections").toInt());
    this->server->setProxy(QNetworkProxy::NoProxy);

    if(!this->server->listen(this->serverAddress, this->serverPort))
        throw new QIrcException(SERVER_RUNTIME_CANNOT_BIND);
    else
        emit sendMessage("[*] Listening on: "+ this->serverAddress.toString() + ":" + QString::number(this->serverPort));

    // Global system room - there are connected all users
    this->rooms.insert("_system", QIrcRoom("_system"));

    this->connections = new QHash<QTcpSocket*, QIrcConnection>();
    this->connections->insert(0, QIrcConnection());
}

void QIrcServer::initTimer(){
    this->serverTimer = new QTimer(this);

    connect(this->serverTimer, SIGNAL(timeout()), this, SLOT(timerTick()));

    this->serverTimer->start(1000 * 30);

    emit sendMessage("[*] Starting server timer");
}

void QIrcServer::validateConfig(){
    if(!this->config)
        return;

    //Listen
    if(!this->config->isValue("Server", "listen"))
        this->config->setValue("Server", "listen", "any");
    else{
        if(this->config->getValue("Server", "listen") != "any"){
            QHostAddress address;

            if(!address.setAddress(this->config->getValue("Server", "listen")))
                throw new QIrcException(SERVER_INVALID_DIRECTIVE_LISTEN);
        }
    }

    //Port
    if(!this->config->isValue("Server", "port"))
        this->config->setValue("Server", "port", "6667");
    else{
        if(this->config->getValue("Server", "port").toInt() == 0)
            throw new QIrcException(SERVER_INVALID_DIRECTIVE_PORT);
    }

    //Connections
    if(!this->config->isValue("Server", "connections"))
        this->config->setValue("Server", "connections", "50");
    else{
        if(this->config->getValue("Server", "connections").toInt() == 0)
            throw new QIrcException(SERVER_INVALID_DIRECTIVE_CONNECTIONS);
    }

    //Hostname
    if(!this->config->isValue("Server", "hostname"))
        this->config->setValue("Server", "hostname", "qirc.dev");

    //User DB file=users.db
    if(!this->config->isValue("Server", "userdb"))
        this->config->setValue("Server", "userdb", "users.db");

    //Server name
    if(!this->config->isValue("Server", "name"))
        this->config->setValue("Server", "name", "qIRC server");

    //Welcome message
    if(!this->config->isValue("Server", "welcome_message"))
        this->config->setValue("Server", "name", "Welcome on qIRC server");

    //MOTD
    if(!this->config->isValue("Server", "motd"))
        this->config->setValue("Server", "motd", "Welcome on private IRC server");
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *  Private slots
 * ----------------------------------------------------------------- */

void QIrcServer::handleNewConnection(){
    QTcpSocket* socket = this->server->nextPendingConnection();

    if(!socket)
        return;

    this->connections->insert(socket, QIrcConnection(QString().sprintf("%08p", socket), socket->peerAddress().toString()));

    emit sendMessage("[" + this->getConnection(socket).getId() + "] Incoming connection from " + socket->peerAddress().toString());

    connect(socket, SIGNAL(readyRead()), this, SLOT(serverReply()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnect()));
}

void QIrcServer::serverReply(){
    QRegExp protoUser("^USER\\ ([A-z0-9]+) ([A-z0-9\\*]+) ([^\\:]+) \\:(.+)$");
    QRegExp protoNick("^NICK ([A-z0-9]+)$");
    QRegExp protoAuth("^AUTH ([A-z0-9]+) (.*)$");
    QRegExp protoReg("^REG ([A-z0-9]+) (.*)$");
    QRegExp protoJoin("^JOIN ([\\#A-z0-9]+) ?(.*)$");
    QRegExp protoPart("^PART ([\\#A-z0-9]+) ?\\:?(.*)$");
    QRegExp protoWho("^WHO ([\\#A-z0-9]+) ?(.*)$");
    QRegExp protoNames("^NAMES ([\\#A-z0-9]+)$");
    QRegExp protoWhois("^WHOIS ([\\#A-z0-9]+) ?(.*)$");
    QRegExp protoPing("^PING (.+)$"); //pouze klient
    QRegExp protoPong("^PONG \\:(.+)$");
    QRegExp protoPrivMsg("^PRIVMSG ([\\#A-z0-9\\\\_]+) \\:(.*)$");
    QRegExp protoQuit("^QUIT( \\:(.*))?$");

    QTcpSocket* socket = qobject_cast<QTcpSocket*>(this->sender());

    //Tohle se v beznem svete nemuze stat..
    if(!this->connections->contains(socket))
        return;

    QByteArray rawData = socket->readAll();

    this->getConnection(socket).getBuffer().append(rawData);

    while(this->getConnection(socket).getBuffer().isReady()){
        rawData = this->getConnection(socket).getBuffer().flush().trimmed();
        emit sendMessage("[" + this->getConnection(socket).getId() + "] [DEBUG] " + rawData);

        //QUIT
        if(protoQuit.indexIn(rawData) != -1){
            QString msg = "";

            if(protoQuit.captureCount() == 2)
                msg = protoQuit.cap(2);

            this->phaseQuit(socket, msg);

            socket->disconnectFromHost();
        }
        //PING
        else if(protoPing.indexIn(rawData) != -1){
            QString msg = protoPing.cap(1);
            //>> :irc.atw-inter.net PONG irc.atw-inter.net :LAG1386480390208491
            this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " PONG " + this->config->getValue("Server", "hostname") + " " + msg);
        }
        //AUTH
        else if(protoAuth.indexIn(rawData) != -1){
            QString user = protoAuth.cap(1);
            QString pass = protoAuth.cap(2);

            this->phaseAuth(socket, user, pass);
        }
        //REG
        else if(protoReg.indexIn(rawData) != -1){
            QString user = protoReg.cap(1);
            QString pass = protoReg.cap(2);

            this->phaseReg(socket, user, pass);
        }
        //AUTH
        else if(protoAuth.indexIn(rawData) != -1){
            QString user = protoAuth.cap(1);
            QString pass = protoAuth.cap(2);

            this->phaseAuth(socket, user, pass);
        }
        //PONG
        else if(protoPong.indexIn(rawData) != -1){
            QString msg = protoPong.cap(1);

            this->getConnection(socket).setTickResponse(msg);
        }
        /*
         * CONNECTED
         */
        else if(this->getConnection(socket).getStatus() >= CONNECTED && this->getConnection(socket).getStatus() < PHASE_AUTHORIZED){
            //USER
            if(protoUser.indexIn(rawData) != -1){
                QString user = protoUser.cap(1);
                QString name = protoUser.cap(2);
                QString address = protoUser.cap(3);
                QString realName = protoUser.cap(4);

                if(this->users->isUser(user) && this->getConnection(socket).getLogin() != user){
                    //444	ERR_NOLOGIN	"<user> :User not logged in"
                    this->socketWrite(socket, QString(":%1 444 %2 :User not logged in")
                        .arg(this->config->getValue("Server", "hostname"))
                        .arg(user));
                }
                else{

                    this->getConnection(socket).setUser(user);
                    this->getConnection(socket).setAddress(address);
                    this->getConnection(socket).setRealName(realName);

                    if(this->getConnection(socket).getStatus() > CONNECTED){
                        this->getConnection(socket).setStatus(PHASE_AUTHORIZED);
                        this->phaseAuthorized(socket);
                    }
                    else
                        this->getConnection(socket).setStatus(PHASE_USER);
                }
            }
            //NICK
            else if(protoNick.indexIn(rawData) != -1){
                QString nick = protoNick.cap(1);

                if(this->users->isUser(nick) && this->getConnection(socket).getLogin() != nick){
                    //444	ERR_NOLOGIN	"<user> :User not logged in"
                    this->socketWrite(socket, QString(":%1 444 %2 :User not logged in")
                        .arg(this->config->getValue("Server", "hostname"))
                        .arg(nick));
                }
                else{

                    if(this->rooms["_system"].getUsers().contains(nick)){
                        this->socketWrite(socket, ":" + this->config->getValue("Server", "name") + " 433 * " + nick + " :Nickname is already in use.");
                    }
                    else{
                        this->rooms["_system"].addUser(nick);

                        this->getConnection(socket).setNick(nick);

                        if(this->getConnection(socket).getStatus() > CONNECTED){
                            this->getConnection(socket).setStatus(PHASE_AUTHORIZED);
                            this->phaseAuthorized(socket);
                        }
                        else
                            this->getConnection(socket).setStatus(PHASE_NICK);
                    }
                }
            }
            else{
                emit sendMessage("[" + this->getConnection(socket).getId() + "] [FIXIT] " + rawData);
            }
        }
        /*
         * AUTHORIZED
         */
        else if(this->getConnection(socket).getStatus() == PHASE_AUTHORIZED){
            //JOIN
            if(protoJoin.indexIn(rawData) != -1){
                QString target = protoJoin.cap(1);

                this->phaseJoin(socket, target);
            }
            //PART
            if(protoPart.indexIn(rawData) != -1){
                QString target = protoPart.cap(1);

                this->phasePart(socket, target);
            }
            //WHO*
            else if(protoWho.indexIn(rawData) != -1){
                QString target = protoWho.cap(1);

                this->phaseWho(socket, target);
            }
            //WHOIS*
            else if(protoWhois.indexIn(rawData) != -1){
                QString target = protoWhois.cap(1);

                this->phaseWhois(socket, target);
            }
            //NAMES
            else if(protoNames.indexIn(rawData) != -1){
                QString target = protoNames.cap(1);

                this->phaseNames(socket, target);
            }
            //PRIVMSG*
            else if(protoPrivMsg.indexIn(rawData) != -1){
                QString target = protoPrivMsg.cap(1);
                QString message = protoPrivMsg.cap(2);

                this->phasePrivmsg(socket, target, message);
            }
            //NICK
            else if(protoNick.indexIn(rawData) != -1){
                QString target = protoNick.cap(1);

                this->phaseNick(socket, target);
            }
        }




    }
}

void QIrcServer::timerTick(){
    emit sendMessage("[*] Timer tick: " + QString::number(this->connections->size()));

    QTcpSocket* socket;
    QList<QTcpSocket*> corpses;

    for(QHash<QTcpSocket*, QIrcConnection>::Iterator p = this->connections->begin(); p != this->connections->end(); p++){

        socket = p.key();

        if(this->getConnection(socket).getStatus() == DISCONNECTED || this->getConnection(socket).getStatus() == ZOMBIE)
            continue;

        if(this->getConnection(socket).getTickRequest() == this->getConnection(socket).getTickResponse()){
            this->getConnection(socket).setTickRequest(QString::number(QDateTime::currentMSecsSinceEpoch()));

            this->socketWrite(p.key(), "PING :" + this->getConnection(socket).getTickRequest());
        }
        else
            corpses.push_back(socket);
    }

    //Recycle corpses
    for(QList<QTcpSocket*>::Iterator p = corpses.begin(); p != corpses.end(); p++){
        emit sendMessage("[" + this->getConnection(socket).getId() + "] [!] Timeout exceeded");

        this->connections->remove(socket);
        (*p)->disconnectFromHost();
    }

    emit sendMessage("[*] Timer end tick: " + QString::number(this->connections->size()));
}

//Handle disconnect
void QIrcServer::clientDisconnect(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(this->sender());

    if(this->connections->contains(socket)){
        this->getConnection(socket).setStatus(DISCONNECTED);

        this->rooms["_system"].removeUser(this->getConnection(socket).getNick());

        emit sendMessage("[" + this->getConnection(socket).getId() + "] [*] Disconnected from " + this->getConnection(socket).getAddress());

        this->connections->remove(socket);
    }

    socket->deleteLater();
    socket->close();
}

// Helpers
void QIrcServer::socketWrite(QTcpSocket* socket, QString data){
    emit sendMessage(">> " + data);
    socket->write(data.toAscii() + "\r\n");
    socket->flush();
}

QTcpSocket* QIrcServer::getSocket(QString nick){
    for(QHash<QTcpSocket*, QIrcConnection>::Iterator p = this->connections->begin(); p != this->connections->end(); p++)
        if((*p).getNick() == nick)
            return p.key();

    return NULL;
}

QIrcConnection& QIrcServer::getConnection(QTcpSocket* socket){
    if(this->connections->contains(socket))
        return (*(this->connections))[socket];

    qDebug() << "Manual creating zombie";

    return (*(this->connections))[0];
}


// Phases

void QIrcServer::phaseAuthorized(QTcpSocket* socket){
    this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " 001 " + this->getConnection(socket).getNick() + " :" + this->config->getValue("Server", "welcome_message") + " "+ this->getConnection(socket).getNick() +"!"+ this->getConnection(socket).getUser() +"@" + this->getConnection(socket).getAddress());
    this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " 002 " + this->getConnection(socket).getNick() + " :Your host is "+ this->config->getValue("Server", "hostname") +", running version 0.1a");
    this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " 003 " + this->getConnection(socket).getNick() + " :This server was created 2013-11-27");
    this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " 251 " + this->getConnection(socket).getNick() + " :There are " + QString::number(this->rooms["_system"].getUsersCount()) + " users");
    this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " 254 " + this->getConnection(socket).getNick() + " " + QString::number(this->rooms.count()) + " :channels formed");

    this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " 375 " + this->getConnection(socket).getNick() + " :- Message of the Day @"+ this->config->getValue("Server", "hostname") +" -");
    this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " 372 " + this->getConnection(socket).getNick() + " :- " + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " 372 " + this->getConnection(socket).getNick() + " :- " + this->config->getValue("Server", "motd"));
    this->socketWrite(socket, ":" + this->config->getValue("Server", "hostname") + " 376 " + this->getConnection(socket).getNick() + " :End of MOTD command.");
}

void QIrcServer::phaseJoin(QTcpSocket* socket, QString target){

    QTcpSocket* ptr;

    if(!this->rooms.contains(target)){
        emit sendMessage("[" + this->getConnection(socket).getId() + "] [*] Creating room " + target);

        QIrcRoom room(target, this->getConnection(socket).getNick());

        this->rooms.insert(target, room);
    }

    this->rooms[target].addUser(this->getConnection(socket).getNick());

    QStringList users = this->rooms[target].getUsers();

    for(QStringList::Iterator p = users.begin(); p != users.end(); p++){
        if((ptr = getSocket(*p))){
            if(*p == this->getConnection(socket).getNick()){
                this->socketWrite(ptr, QString(":%1!~%2@%3 JOIN %4")
                    .arg(this->getConnection(socket).getNick())
                    .arg(this->getConnection(socket).getUser())
                    .arg(this->getConnection(socket).getAddress())
                    .arg(target));

                this->phaseNames(ptr, target);

            }
            else{
                this->socketWrite(ptr, QString(":%1!~%2@%3 JOIN :%4")
                    .arg(this->getConnection(socket).getNick())
                    .arg(this->getConnection(socket).getUser())
                    .arg(this->getConnection(socket).getAddress())
                    .arg(target));

                this->phaseNames(ptr, target);
            }
        }
    }
}

void QIrcServer::phasePart(QTcpSocket* socket, QString target){
    QTcpSocket* ptr;

    if(!this->rooms.contains(target))
        return;

    this->rooms[target].removeUser(this->getConnection(socket).getNick());

    QStringList users = this->rooms[target].getUsers();

    for(QStringList::Iterator p = users.begin(); p != users.end(); p++){
        if((ptr = getSocket(*p))){
            //if(*p == this->getConnection(socket).getNick()){
                this->socketWrite(ptr, QString(":%1!~%2@%3 PART %4")
                    .arg(this->getConnection(socket).getNick())
                    .arg(this->getConnection(socket).getUser())
                    .arg(this->getConnection(socket).getAddress())
                    .arg(target));
            //}
        }
    }
}

void QIrcServer::phaseWho(QTcpSocket* socket, QString target){
    QTcpSocket* ptr;

    //Room
    if(target.startsWith('#')){
        if(this->rooms.contains(target)){
            QStringList users = this->rooms[target].getUsers();

            for(QStringList::Iterator p = users.begin(); p != users.end(); p++){
                if((ptr = getSocket(*p))){
                    //:irc.felk.cvut.cz 352 Filgron #fit11 ~filgy 176.74.128.47 irc.felk.cvut.cz Filgron H :0 203F Lord destructor
                    this->socketWrite(socket, QString(":%1 352 %2 %3 ~%4 %5 %6 %7 H :0 203F %8")
                        .arg(this->config->getValue("Server", "hostname"))
                        .arg(this->getConnection(socket).getNick())
                        .arg(target)

                        .arg(this->getConnection(ptr).getUser())
                        .arg(this->getConnection(ptr).getAddress())
                        .arg(this->config->getValue("Server", "hostname"))
                        .arg(this->getConnection(ptr).getNick())
                        .arg(this->getConnection(ptr).getRealName()));
                }
            }
        }

        //:irc.felk.cvut.cz 315 Filgron #fit11 :End of WHO list.
        this->socketWrite(socket, QString(":%1 315 %2 %3 :End of WHO list.")
            .arg(this->config->getValue("Server", "hostname"))
            .arg(this->getConnection(socket).getNick())
            .arg(target));
    }
    //Person
    else{
        if((ptr = getSocket(target))){
            //:irc.felk.cvut.cz 352 Filgron #fit11 ~filgy 176.74.128.47 irc.felk.cvut.cz Filgron H :0 203F Lord destructor
            this->socketWrite(socket, QString(":%1 352 %2 %3 ~%4 %5 %6 %7 H :0 203F %8")
                .arg(this->config->getValue("Server", "hostname"))
                .arg(this->getConnection(socket).getNick())
                .arg(target)

                .arg(this->getConnection(ptr).getUser())
                .arg(this->getConnection(ptr).getAddress())
                .arg(this->config->getValue("Server", "hostname"))
                .arg(this->getConnection(ptr).getNick())
                .arg(this->getConnection(ptr).getRealName()));
        }

        //:irc.felk.cvut.cz 315 Filgron #fit11 :End of WHO list.
        this->socketWrite(socket, QString(":%1 315 %2 %3 :End of WHO list.")
            .arg(this->config->getValue("Server", "hostname"))
            .arg(this->getConnection(socket).getNick())
            .arg(target));
    }
}

void QIrcServer::phaseWhois(QTcpSocket* socket, QString target){
    QTcpSocket* ptr;

    if((ptr = getSocket(target))){
        //:irc.felk.cvut.cz 311 Filgron Filgy ~quassel angie.sniff.ws * :Lord destroyer
        this->socketWrite(socket, QString(":%1 311 %2 %3 ~%4 %5 * :%6")
            .arg(this->config->getValue("Server", "hostname"))
            .arg(this->getConnection(socket).getNick())
            .arg(target)
            .arg(this->getConnection(ptr).getUser())
            .arg(this->getConnection(ptr).getAddress())
            .arg(this->getConnection(ptr).getRealName()));

        //:irc.felk.cvut.cz 312 Filgron Filgy irc.nfx.cz :Neutral czFree eXchange
        this->socketWrite(socket, QString(":%1 312 %2 %3 %4 :%5")
            .arg(this->config->getValue("Server", "hostname"))
            .arg(this->getConnection(socket).getNick())
            .arg(target)
            .arg(this->config->getValue("Server", "hostname"))
            .arg(this->config->getValue("Server", "name")));
    }
    //:irc.felk.cvut.cz 318 Filgron Filgy :End of WHOIS list.
    this->socketWrite(socket, QString(":%1 318 %2 %3 :End of WHOIS list")
        .arg(this->config->getValue("Server", "hostname"))
        .arg(this->getConnection(socket).getNick())
        .arg(target));
}

void QIrcServer::phaseNames(QTcpSocket* socket, QString target){
    if(target.startsWith('#')){
        QStringList users = this->rooms[target].getUsers();

        if(this->rooms.contains(target)){
            this->socketWrite(socket, QString(":%1 353 %2 = %3 :%4")
                .arg(this->config->getValue("Server", "hostname"))
                .arg(this->getConnection(socket).getNick())
                .arg(target)
                .arg(users.join(" ")));
        }

        this->socketWrite(socket, QString(":%1 366 %2 %3 :End of NAMES list.")
            .arg(this->config->getValue("Server", "hostname"))
            .arg(this->getConnection(socket).getNick())
            .arg(target));
    }
}

void QIrcServer::phasePrivmsg(QTcpSocket* socket, QString target, QString message){

    QTcpSocket* ptr;

    //Room
    if(target.startsWith('#')){
        if(this->rooms.contains(target)){
            QStringList users = this->rooms[target].getUsers();

            for(QStringList::Iterator p = users.begin(); p != users.end(); p++){
                if(*p != this->getConnection(socket).getNick())
                if((ptr = getSocket(*p))){
                    this->socketWrite(ptr, QString(":%1!~%2@%3 PRIVMSG %4 :%5")
                        .arg(this->getConnection(socket).getNick())
                            .arg(this->getConnection(socket).getUser())
                            .arg(this->getConnection(socket).getAddress())
                            .arg(target)
                            .arg(message));
                }
            }
        }
        else{
            this->socketWrite(socket, QString(":%1 401 %2 %3 :No such nick/channel")
                .arg(this->config->getValue("Server", "hostname"))
                .arg(this->getConnection(socket).getNick())
                .arg(target));
        }
    }
    //Person
    else{
        if((ptr = this->getSocket(target))){
            this->socketWrite(ptr, QString(":%1!~%2@%3 PRIVMSG %4 :%5")
                .arg(this->getConnection(socket).getNick())
                    .arg(this->getConnection(socket).getUser())
                    .arg(this->getConnection(socket).getAddress())
                    .arg(target)
                    .arg(message));
        }
        else{
            this->socketWrite(socket, QString(":%1 401 %2 %3 :No such nick/channel")
                .arg(this->config->getValue("Server", "hostname"))
                .arg(this->getConnection(socket).getNick())
                .arg(target));
        }
    }
}

void QIrcServer::phaseQuit(QTcpSocket* socket, QString message){
    QTcpSocket* ptr;

    QStringList survivors;

    for(QHash<QString, QIrcRoom>::Iterator p = this->rooms.begin(); p != this->rooms.end(); p++){
        QStringList users = (*p).getUsers();

        if(users.contains(this->getConnection(socket).getNick())){
            (*p).removeUser(this->getConnection(socket).getNick());

            for(QStringList::Iterator q = users.begin(); q != users.end(); q++){
                if(!survivors.contains(*q))
                    survivors.append(*q);
            }
        }
    }

    for(QStringList::Iterator p = survivors.begin(); p != survivors.end(); p++){
        if((ptr = this->getSocket(*p))){
            this->socketWrite(ptr, QString(":%1!~%2@%3 QUIT :%4")
                .arg(this->getConnection(socket).getNick())
                .arg(this->getConnection(socket).getUser())
                .arg(this->getConnection(socket).getAddress())
                .arg(message));
        }
    }
}

void QIrcServer::phaseNick(QTcpSocket* socket, QString target){

    QTcpSocket* ptr;

if(this->users->isUser(target) && this->getConnection(socket).getLogin() != target.toLower()){
        //444	ERR_NOLOGIN	"<user> :User not logged in"
        this->socketWrite(socket, QString(":%1 444 %2 :User not logged in")
            .arg(this->config->getValue("Server", "hostname"))
            .arg(target));
}
else{
    if(this->rooms["_system"].getUsers().contains(target)){
        this->socketWrite(socket, QString(":%1 433 %2 :Nickname is already in use.")
            .arg(this->config->getValue("Server", "name"))
            .arg(target));
    }
    else{
        this->rooms["_system"].removeUser(this->getConnection(socket).getNick());
        this->rooms["_system"].addUser(target);

        QStringList survivors;

        for(QHash<QString, QIrcRoom>::Iterator p = this->rooms.begin(); p != this->rooms.end(); p++){
            QStringList users = (*p).getUsers();

            if(users.contains(this->getConnection(socket).getNick())){
               (*p).removeUser(this->getConnection(socket).getNick());
               (*p).addUser(target);

               for(QStringList::Iterator q = users.begin(); q != users.end(); q++){
                   if(!survivors.contains(*q))
                       survivors.append(*q);
               }
            }
        }

        for(QStringList::Iterator p = survivors.begin(); p != survivors.end(); p++){
            if((ptr = this->getSocket(*p))){
                this->socketWrite(ptr, QString(":%1!~%2@%3 NICK :%4")
                    .arg(this->getConnection(socket).getNick())
                    .arg(this->getConnection(socket).getUser())
                    .arg(this->getConnection(socket).getAddress())
                    .arg(target));
            }
        }

        this->getConnection(socket).setNick(target);
    }
}
}

void QIrcServer::phaseAuth(QTcpSocket* socket, QString user, QString pass){
    if(!this->users->isUser(user)){
        this->socketWrite(socket, QString(":%1 451 :You have not registered")
            .arg(this->config->getValue("Server", "hostname")));
    }
    else if(!this->users->isValid(user, pass)){
        this->socketWrite(socket, QString(":%1 464 :Password incorrect")
            .arg(this->config->getValue("Server", "hostname")));
    }
    else{
        this->getConnection(socket).setLogin(user);
        emit sendMessage("[" + this->getConnection(socket).getId() + "] [*] Login user " + user);
    }
}

void QIrcServer::phaseReg(QTcpSocket* socket, QString user, QString pass){
    if(this->users->isUser(user)){
        this->socketWrite(socket, QString(":%1 462 :Unauthorized command (already registered)")
            .arg(this->config->getValue("Server", "hostname")));
    }
    else{
        this->users->addUser(user, pass);
        emit sendMessage("[" + this->getConnection(socket).getId() + "] [*] Created user " + user);
    }
}
