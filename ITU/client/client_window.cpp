#include "client_window.h"
#include "ui_client_window.h"
#include "about.h"
#include "new_server.h"
#include "join_chan.h"
#include "new_account.h"
#include "change_account.h"

Client_Window::Client_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client_Window)
{
    ui->setupUi(this);
    ui->chat_smilies->hide();
    on_actionNew_serv_triggered();

    socket = NULL;
    amIConnected = false;
    if (!dir.exists("log"))
        dir.mkdir("log");
    //dir.cd("log");

}

Client_Window::~Client_Window()
{
    delete ui;
    if(socket != NULL)
        saveToLog();

}

void Client_Window::on_actionExit_triggered()
{
    if(socket != NULL)
        saveToLog();
    this->close();
}

void Client_Window::on_actionNew_serv_triggered()
{
    New_Server *s = new New_Server();
    s->show();
    connect(s, SIGNAL(newServer(QString)), this, SLOT(connectToServer(QString)));
}

void Client_Window::on_actionJoin_chan_triggered()
{
    Join_chan *c = new Join_chan();
    c->show();
    connect(c, SIGNAL(newChannel(QString)), this, SLOT(connectToChannel(QString)));
    //socket->write("JOIN #testroom \r\n");
}

void Client_Window::on_actionNew_acc_triggered()
{
    New_account *acc = new New_account();
    acc->show();
}

void Client_Window::on_actionChange_acc_triggered()
{
    Change_account *acc = new Change_account();
    acc->show();
}

void Client_Window::on_actionClose_serv_triggered()
{
    // zavreni kanalu
    if (amIConnected) {
        writeLine("QUIT Good bye"); // Good bye is optional message
        writeLog(serverAddres, "Connection Close");
        socket->flush();
        socket->disconnect();
        socket->close();
        socket = NULL;
        //ui->chat_text->clear();

        saveToLog();
        ui->chat_active->clear();
        amIConnected = false;
    }
}

void Client_Window::on_actionPart_chan_triggered()
{
    // zavreni aktivniho chatu
    if(chatText.count() == 1)
    {
        ui->chat_text->append("Nejste pripojeni k zadnemu kanalu!");
        return;
    }
    //qDebug() << ";aaaa" <<chatText.value(activChannel);
    writeLine("PART "+ activChannel);
    writeLog(activChannel,chatText.value(activChannel));
    chatText.remove(activChannel);
    actualizationChannel();
}

void Client_Window::on_actionDelete_acc_triggered()
{
    QSettings settings("init.ini", QSettings::NativeFormat);
    QString accText = settings.value("Account", "").toString();
    if (accText.isEmpty())
        return;
    settings.setValue("Account", "");

    QFile file("nicks.txt");
    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        QStringList save;
        while (!stream.atEnd()) {
            QString nick = stream.readLine();
            if (nick.compare(accText) != 0)
                save.append(nick);
        }
        file.close();
        file.remove();
        file.open(QIODevice::WriteOnly);
        for (int i = 0; i < save.size(); ++i)
            stream << save.at(i) << endl;
        file.close();
    }
}

void Client_Window::on_actionHelp_triggered()
{
    QDesktopServices::openUrl(QUrl("http://www.irchelp.org/")); //trololo
}

void Client_Window::on_actionAbout_triggered()
{
    About *a = new About();
    a->show();
}

void Client_Window::connectToServer(QString password)
{
    writeLog(serverAddres, "Connecting...");
    this->password = password;
    loadSetting();

    /*if (this->serverAddres.isEmpty() || this->nick.isEmpty())
        return;*/

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connectEstablish()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));

    amIConnected = true;
    socket->connectToHost(this->serverAddres, this->port);
}

void Client_Window::connectEstablish() {
    writeLog(serverAddres, "Connected to: " + socket->peerName() + " on port " + QString("%1").arg(this->port));

    writeLine(QString("NICK %1").arg(this->nick));
    writeLine(QString("USER %1 %2 %3 :%4").arg(this->nick,this->nick,this->nick,this->nick));

    /*if (!this->password.isEmpty())
        this->writeLine("NS ID "+this->password);*/
    ui->chat_active->clear();
    ui->chat_nicks->clear();
    ui->chat_text->clear();

    emit Connected();
}

void Client_Window::readData() {

    while (socket->canReadLine())
    {
        QString line = QString::fromUtf8(socket->readLine());
        line.remove("\r\n");

        qDebug() << "Recv:" << line;
        //ui->chat_text->append(line);

        parseLine(line);
    }
}

void Client_Window::parseLine(QString line)
{
    QStringList pieces = line.split( " " );
    //ui->chat_text->append(line);
    if(pieces.at(0) == "PING")
    {
        writeLine("PONG "+pieces.at(1));
        return;
    }
    int lastchar = pieces.count();

    if(pieces[lastchar-1].length() == 0)
    {
        pieces.removeAt(lastchar-1);
    }

    bool isNum;
    int raw = pieces.at(1).toInt(&isNum);

    QString Nick = pieces.at(0).section('!', 0, 0).mid(1);

    QString Command = pieces.at(1);

    QString Target = pieces.at(2);

    if (!isNum) {
        if(Nick == "IRC")
        {
            ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick.leftJustified(16, ' ', true) + pieces.at(3));
            return;
        }
        QString msg;
        if(pieces.count() >3)
        {
            QStringList msgList = pieces.mid(3);
            msg = msgList.join(" ").mid(1);
        }

        if (Command.contains("JOIN"))
        {
            if(this->nick == Nick)
            {
                QString s = ui->chat_text->toPlainText();
                chatText.insert(activChannel, s);
                ui->chat_text->clear();

                this->activChannel = Target.mid(1);
                chatText.insert(activChannel, "");

                ui->chat_active->addItem(activChannel);
                int position = ui->chat_active->count();
                ui->chat_active->setCurrentRow(position-1);
                ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick + " joined " + Target.mid(1));
                return;
            }
            if(activChannel == Target.mid(1))
            {
                ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick + " joined " + Target.mid(1));
                writeLine(QString("NAMES %1").arg(Target.mid(1)));
            }
            else
                chatText[Target.mid(1)].append("\n"+time.currentTime().toString("HH:mm:ss ")+Nick + " joined " + Target.mid(1));
        }

        if (Command.contains("KICK")) {
            if(activChannel == Target)
            {
                ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick + " left " + Target.mid(1));
                writeLine(QString("NAMES %1").arg(Target.mid(1)));
            }
        }

        if (Command.contains("NICK")) {
            if(Nick != this->nick)
            {
                ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick + " change to " + Target);
                writeLine(QString("NAMES %1").arg(this->activChannel));
            }
            else
            {
                this->nick = Target.mid(1);
                writeLine(QString("NAMES %1").arg(this->activChannel));
            }
        }

        if (Command.contains("PART")) {
            if(this->nick == Nick)
            {
                return;
            }
            if(activChannel == Target)
            {
                ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick + " left " + Target);
                writeLine(QString("NAMES %1").arg(Target));
            }
            else
                chatText[Target].append("\n"+time.currentTime().toString("HH:mm:ss ")+Nick + " left " + Target);

        }

        if (Command.contains("PRIVMSG")) {
            if(Target.compare(this->nick) == 0)
            {
                if(chatText[Nick].length() < 1)
                {
                    //QString s = ui->chat_text->toPlainText();
                    //chatText.insert(activChannel, s);

                    //ui->chat_text->clear();
                    //this->activChannel = Nick;
                    chatText.insert(Nick, "");

                    ui->chat_active->addItem(Nick);
                    //int position = ui->chat_active->count();
                    //ui->chat_active->setCurrentRow(position-1);
                    //ui->chat_text->append(Nick.leftJustified(16, ' ', true) + msg);
                    chatText[Nick].append(time.currentTime().toString("HH:mm:ss ")+Nick.leftJustified(16, ' ', true) + msg);
                }
                else
                {
                    if(activChannel.compare(Nick) == 0)
                    {
                        ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick.leftJustified(16, ' ', true) + msg);
                    }
                    else
                        chatText[Nick].append("\n"+time.currentTime().toString("HH:mm:ss ")+Nick.leftJustified(16, ' ', true) + msg);
                }
            }
            else
            {
                if(activChannel.compare(Target) == 0)
                {
                    ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick.leftJustified(16, ' ', true) + msg);
                }
                else
                    chatText[Target].append("\n"+time.currentTime().toString("HH:mm:ss ")+Nick.leftJustified(16, ' ', true) + msg);
            }
        }
        if (Command.contains("MODE")) {
            ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick.leftJustified(16, ' ', true) + msg);
        }
        if (Command.contains("NOTICE")) {
            serverName = Nick;
            activChannel = Nick;
            ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+Nick.leftJustified(16, ' ', true) + msg);
            chatText.insert(serverName, "");
            int position = ui->chat_active->count();
            if(position == 0)
            {
                ui->chat_active->addItem(activChannel);
                ui->chat_active->setCurrentRow(0);
            }
        }

    }

    else
    {
        //divny cisla https://www.alien.net.au/irc/irc2numerics.html
        //poslani uzivatelu a zprav ktere si vymenuji servery

        if(raw == 353)
        {
            ui->chat_nicks->clear();
            //qDebug() << pieces.count();
            for(int a=5; a < pieces.count(); a++ )
            {
                if((pieces[a].mid(0,1) == ":") || (pieces[a].mid(0,1) == "~") || (pieces[a].mid(0,1) == "&"))
                    ui->chat_nicks->addItem(pieces[a].mid(1));
                else
                    ui->chat_nicks->addItem(pieces[a]);
            }
        }
        if(raw == 433)
        {
            ui->chat_text->append("Nickname is already in use.");
        }
    }
}

void Client_Window::writeLine(const QString &line)
{
    qDebug() << "Send:" << line;
    socket->write(QString("%1\r\n").arg(line).toAscii());
}

void Client_Window::loadSetting()
{
    QSettings settings(QApplication::applicationDirPath(), "init.ini");
    this->serverAddres = settings.value("Server", "").toString();
    this->port = settings.value("Port", "").toInt();
    this->nick = settings.value("Account", "").toString();
}

void Client_Window::on_send_button_pressed()
{
    if(activChannel == serverName && ui->chat_input->text().mid(0, 1).compare("/") != 0)
    {
        ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+"Pripojte se na kanal!");
        ui->chat_input->clear();
    }
    if(ui->chat_input->text() == "")
        return;
    if(socket == NULL)
    {
        ui->chat_input->clear();
        return;
    }

    QString text = ui->chat_input->text();
    if(text.mid(0,1) != "/")
    {
        writeLine("PRIVMSG "+ activChannel + " :" + text);
        ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+this->nick.leftJustified(16, ' ', true) + text);
    }
    else {
        if (text.mid(1,4).compare("JOIN", Qt::CaseInsensitive) == 0) {
            if (text.at(6) == '#')
                connectToChannel(text.mid(6));
            else
                connectToChannel("#"+text.mid(6));
        }
        else if (text.mid(1,2).compare("ME", Qt::CaseInsensitive) == 0) {
            if (activChannel == serverName)
                ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+"Pripojte se na kanal!");
            else {
                text = text.mid(4);
                writeLine("PRIVMSG "+ activChannel + " :\u0001ACTION " + text + "\u0001");//"PRIVMSG #dreamincode :\u0001ACTION is awesome\u0001"
                ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+this->nick+" "+text); /// tady nevim, jestli posilam spravne zpravu na server////////////
            }
        }
        else if (text.mid(1,3).compare("MSG", Qt::CaseInsensitive) == 0 || text.mid(1, 6).compare("NOTICE", Qt::CaseInsensitive) == 0) {
            QString s = ui->chat_text->toPlainText();
            chatText.insert(activChannel, s);
            ui->chat_text->clear();

            int s_nick = text.indexOf(" ")+1;
            int e_nick = text.indexOf(" ", s_nick);
            activChannel = QString("%1").arg(text.mid(s_nick, e_nick-s_nick));
            text = text.mid(e_nick+1);
            //qDebug() << text;
            chatText.insert(activChannel, "");
            ui->chat_active->addItem(activChannel);
            int position = ui->chat_active->count();
            ui->chat_active->setCurrentRow(position-1);
            ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+this->nick + " to " + activChannel);
            writeLine("PRIVMSG "+ activChannel + " " + text);
            ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+this->nick.leftJustified(16, ' ', true) + text);
        }
        else if (text.mid(1, 4).compare("NICK", Qt::CaseInsensitive) == 0) {
            writeLine(QString("NICK %1").arg(text.mid(6)));
        }
        else if (text.mid(1, 4).compare("PART", Qt::CaseInsensitive) == 0) {
            if (activChannel == serverName)
                ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+"Pripojte se na kanal!");
            else {
                on_actionPart_chan_triggered();
            }
        }
        else if (text.mid(1, 4).compare("QUIT", Qt::CaseInsensitive) == 0) {
            on_actionClose_serv_triggered();
        }
        else if (text.mid(1, 5).compare("WHOIS", Qt::CaseInsensitive) == 0) {
            writeLine(QString("WHOIS %1").arg(text.mid(6)));

        }
    }
    ui->chat_input->clear();
}

void Client_Window::connectToChannel(QString channel)
{
    if(socket != NULL)
    {
        writeLine("JOIN " + channel);
    }
    else
    {
        ui->chat_text->append("Nejsi pripojen k serveru!");
    }
}

void Client_Window::on_chat_active_itemClicked(QListWidgetItem *item)
{
    item=item;
    QString s = ui->chat_text->toPlainText();
    chatText.insert(activChannel, s);

    ui->chat_text->clear();
    activChannel = QString("%1").arg(ui->chat_active->currentItem()->text());
    ui->chat_text->append(chatText.value(activChannel));
    if(activChannel == serverName)
        ui->chat_nicks->clear();
    else
        writeLine(QString("NAMES %1").arg(this->activChannel));
}

void Client_Window::actualizationChannel()
{
    ui->chat_active->clear();
    /*if(chatText.count() == 1)
    {
        activChannel = serverName;
        qDebug() << "nastaveno na server";
        ui->chat_text->clear();
        ui->chat_text->append(chatText.value(serverName));
        return;
    }*/
    QStringList all = chatText.keys();

    //int row = all.indexOf(QRegExp(serverName, Qt::CaseInsensitive, QRegExp::Wildcard));
    //all.removeAt(row);

    ui->chat_active->addItems(all);
    ui->chat_active->setCurrentRow(0);
    ui->chat_text->clear();
    activChannel = QString("%1").arg(ui->chat_active->currentItem()->text());
    ui->chat_text->append(chatText.value(activChannel));
}

void Client_Window::writeLog(QString filename, QString text)
{
    QFile file("log/" + filename + ".log");
    if(!file.exists()) {
        if(!file.open(QIODevice::WriteOnly))
            qDebug() << "Error while opening a file" << endl;
    }
    else {
        if(!file.open(QIODevice::Append))
            qDebug() << "Error while opening a file" << endl;
    }
    QTextStream stream(&file);
    qDebug() << filename << text;
    stream << text << endl;
    file.close();
}

void Client_Window::saveToLog()
{
    chatText.remove(serverName);
    chatText.insert(activChannel,ui->chat_text->toPlainText());
    QStringList channel= chatText.keys();
    foreach(QString chan, channel)
    {
        writeLog(chan, chatText[chan]);
    }
    chatText.clear();
    ui->chat_text->clear();
}

void Client_Window::on_chat_input_returnPressed()
{
    on_send_button_pressed();
}

void Client_Window::on_chat_nicks_itemClicked(QListWidgetItem *item)
{
    if (ui->chat_input->isModified())
        ui->chat_input->insert(item->text());
    else
        ui->chat_input->insert(item->text()+": ");
}

void Client_Window::on_chat_nicks_itemDoubleClicked(QListWidgetItem *item)
{
    QString s = ui->chat_text->toPlainText();
    chatText.insert(activChannel, s);
    ui->chat_text->clear();

    activChannel = QString("%1").arg(item->text());
    chatText.insert(activChannel, "");
    ui->chat_active->addItem(activChannel);
    int position = ui->chat_active->count();
    ui->chat_active->setCurrentRow(position-1);
    ui->chat_text->append(time.currentTime().toString("HH:mm:ss ")+this->nick + " to " + item->text());
}

void Client_Window::on_smiles_button_clicked()
{
    if (ui->chat_smilies->isVisible())
        ui->chat_smilies->hide();
    else
        ui->chat_smilies->show();
}

void Client_Window::on_smile_00_clicked()
{
    ui->chat_input->insert(" :) ");
}

void Client_Window::on_smile_01_clicked()
{
    ui->chat_input->insert(" ;) ");
}

void Client_Window::on_smile_02_clicked()
{
    ui->chat_input->insert(" 8) ");
}

void Client_Window::on_smile_03_clicked()
{
    ui->chat_input->insert(" %) ");
}

void Client_Window::on_smile_04_clicked()
{
    ui->chat_input->insert(" =) ");
}

void Client_Window::on_smile_10_clicked()
{
    ui->chat_input->insert(" :( ");
}

void Client_Window::on_smile_11_clicked()
{
    ui->chat_input->insert(" :/ ");
}

void Client_Window::on_smile_12_clicked()
{
    ui->chat_input->insert(" :@ ");
}

void Client_Window::on_smile_13_clicked()
{
    ui->chat_input->insert(" :C ");
}

void Client_Window::on_smile_14_clicked()
{
    ui->chat_input->insert(" :< ");
}

void Client_Window::on_smile_20_clicked()
{
    ui->chat_input->insert(" :D ");
}

void Client_Window::on_smile_21_clicked()
{
    ui->chat_input->insert(" :P ");
}

void Client_Window::on_smile_22_clicked()
{
    ui->chat_input->insert(" :O ");
}

void Client_Window::on_smile_23_clicked()
{
    ui->chat_input->insert(" :X ");
}

void Client_Window::on_smile_24_clicked()
{
    ui->chat_input->insert(" :S ");
}

void Client_Window::on_smile_30_clicked()
{
    ui->chat_input->insert(" :* ");
}

void Client_Window::on_smile_31_clicked()
{
    ui->chat_input->insert(" :3 ");
}

void Client_Window::on_smile_32_clicked()
{
    ui->chat_input->insert(" :p ");
}

void Client_Window::on_smile_33_clicked()
{
    ui->chat_input->insert(" :*) ");
}

void Client_Window::on_smile_34_clicked()
{
    ui->chat_input->insert(" :} ");
}

void Client_Window::on_smile_40_clicked()
{
    ui->chat_input->insert(" :'( ");
}

void Client_Window::on_smile_41_clicked()
{
    ui->chat_input->insert(" :$ ");
}

void Client_Window::on_smile_42_clicked()
{
    ui->chat_input->insert(" o_O ");
}

void Client_Window::on_smile_43_clicked()
{
    ui->chat_input->insert(" [:-) ");
}

void Client_Window::on_smile_44_clicked()
{
    ui->chat_input->insert(" O:-) ");
}
