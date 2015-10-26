/**
  * @file qmain.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QMain
  */

#include "qmain.h"
#include "ui_qmain.h"

QMain::QMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::QMain){
    ui->setupUi(this);

    this->setWindowTitle("Labyrinth2015");
    this->setPalette(QPalette(Qt::white));
    this->setAutoFillBackground(true);
    this->resize(640, 480);
    this->setMaximumSize(640, 480);
    this->setMinimumSize(640, 480);
    this->timer = NULL;
    this->timerDialog = NULL;
    this->desk = new QCustomWrapper(this);

    this->setCentralWidget(this->desk);

    this->log = new QDebugLog(this);

    connect(this, SIGNAL(logMessage(QString, int)), this->log, SLOT(receiveMessage(QString)));
}

QMain::~QMain(){
    delete ui;

    if(this->worker){
        this->worker->terminate();
        //delete this->worker;
    }

    if(this->timerDialog){
        delete this->timerDialog;
    }

    this->worker = NULL;
    this->timerDialog = NULL;

    delete this->log;
    this->log = NULL;
}

void QMain::initTimer(int ms){
    if(this->timer){
        this->timer->stop();
        delete this->timer;
    }

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerTick()));
    this->timer->start(ms);
}

void QMain::handleShowHelp(){
    QDialogHelp dialog(this);

    dialog.exec();
}

void QMain::handleShowConnect(){
    dialogConnect dialog(this);

    if(dialog.exec() == QDialog::Accepted){
        this->hostname = dialog.getHostname();
        this->port = dialog.getPort();
    }
    else
        return;

    this->worker = new QCLI(this->hostname, this->port);
    connect(this->worker, SIGNAL(finished()), this, SLOT(handleWorkerDied()));
    connect(this->worker, SIGNAL(sendMessage(QString)), this, SLOT(handleWorkerMessage(QString)));
    connect(this, SIGNAL(sendMessage(QString)), this->worker, SLOT(receiveMessage(QString)));

    this->worker->start();

    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
    emit logMessage("Connected to remote server");
    ui->actionInvite_players->setEnabled(true);
    ui->actionExit->setEnabled(false);

    if(this->timerDialog){
        delete this->timerDialog;
    }
    this->timerDialog = new QDialogLoading(this);
    this->timerDialog->hide();
    this->desk->showLabel(false);

    this->initTimer(1000);
}

void QMain::handleDisconnect(){
    this->timer->stop();

    if(this->worker){
        if(ui->actionSave_game->isEnabled())
            emit sendMessage("STOPGAME;;");

        emit sendMessage("EXIT;;");
        //this->worker->terminate();
        emit logMessage("Disconnecting..");
    }
    else
        emit logMessage("You are not connected to the server");
}

void QMain::handleShowInvite(){
    if(this->worker){
        emit sendMessage("CLIENTCOUNT;;");
        emit sendMessage("CLIENTLIST;;");
        emit logMessage("Getting info about connected clients..");
        this->timerDialog->exec();
    }
    else
        emit logMessage("You are not connected to the server");
}
void QMain::handleShowNewGame(){
    QDialogNewGame dialog(this);

    if(dialog.exec() == QDialog::Accepted){
        int size = dialog.getSize();
        int players = dialog.getPlayers();
        int cards = dialog.getCards();

        emit sendMessage("CREATEGAME;"+QString::number(size)+","+QString::number(cards)+","+QString::number(players)+";");
        emit logMessage("Creating game.."+QString::number(size)+","+QString::number(cards)+","+QString::number(players));
        ui->actionSave_game->setEnabled(true);
        this->timerDialog->show();

    }
    else
        emit logMessage("New game wasn't created");

}
void QMain::handleShowLoadGame(){
    if(this->worker){
        emit sendMessage("GAMES;;");
        emit logMessage("Getting stored games..");
        this->timerDialog->exec();
    }
    else
        emit logMessage("You are not connected to the server");

}
void QMain::handleShowSaveGame(){
    if(this->worker){
        bool ok;
        QString gameName = QInputDialog::getText(this, "Labyrinth2015 - Save game", "Filename", QLineEdit::Normal, "", &ok);

        if(ok && gameName.isEmpty()){
            QMessageBox::warning(this, "Labyrinth2015 - Error", "Please specify name of the game");
            emit logMessage("Please specify name of the game");
        }
        else if(ok && !gameName.isEmpty()){
            gameName = gameName.replace(';', "");
            gameName = gameName.replace(',', "");
            gameName = gameName.replace(' ', "");

            emit sendMessage("SAVEGAME;"+gameName+";");
        }
        else
            emit logMessage("Game was not saved");
    }
    else
        emit logMessage("You are not connected to the server");
}

void QMain::handleWorkerMessage(QString message){
    emit logMessage(message);
}

void QMain::handleWorkerDied(){

    emit logMessage("Disconnected");

    if(this->timer)
        this->timer->stop();

    if(this->desk)
        this->desk->clearDesk();

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionInvite_players->setEnabled(false);
    ui->actionNew_game->setEnabled(false);
    ui->actionLoad_game->setEnabled(false);
    ui->actionExit->setEnabled(true);
    ui->actionSave_game->setEnabled(false);
}

void QMain::debugToggle(){
    if(this->log->isHidden())
        this->log->show();
    else
        this->log->hide();
}

void QMain::timerTick(){
    QMessageBox::StandardButton reply;
    QString message;
    int invitedBy;
    vector<string> games;

    emit logMessage("Checking events..");
    if(this->worker){
        while(this->worker->getClient()->getEvents() > 0){
            CLIEvent event = this->worker->getClient()->getEvent();

            switch(event){
                case E_CLIENTLIST:
                    emit logMessage("New event: E_CLIENTLIST");
                    if(this->timerDialog)
                        this->timerDialog->accept();

                    if(this->worker->getClient()->getClientCount() == 1){
                        QMessageBox::warning(this, "Labyrinth2015", "There is no player online");
                        emit logMessage("There is no player online");
                    }
                    else{
                        QDialogInvite dialog(this->worker->getClient()->getClients(), this->worker->getClient()->getCurrentClient(), this);

                        if(dialog.exec() == QDialog::Accepted){
                            QStringList players = dialog.getPlayers();
                            if(players.size() > 0){
                                emit sendMessage("INVITE;"+players.join(",")+";");

                                emit logMessage("Invitation has been sent");
                            }
                            else{
                                QMessageBox::warning(this, "Labyrinth2015", "You have to select at least one player");
                            }
                        }
                        else
                            emit logMessage("Invitation has been cancelled");
                    }
                    break;
                case E_INVITE:
                    emit logMessage("New event: E_INVITE");
                    invitedBy = this->worker->getClient()->getInvitedBy();

                    reply = QMessageBox::question(this, "Labyrinth2015 - Invitation", "Player "+QString::number(invitedBy)+" invited you to game, do you accept?", QMessageBox::Yes|QMessageBox::No);

                    if(reply == QMessageBox::Yes){
                        emit sendMessage("ACCEPT;"+QString::number(invitedBy)+";");
                        emit logMessage("Invitation has been accepted");
                    }
                    else{
                        emit logMessage("Invitation has been declined");
                    }

                    break;
                case E_ACCEPTED:
                    emit logMessage("New event: E_ACCEPTED");
                    message = QString::fromStdString(this->worker->getClient()->getMessage());
                    QMessageBox::information(this, "Labyrinth2015 - Notice", message);
                    emit logMessage(message);
                    ui->actionLoad_game->setEnabled(true);
                    ui->actionNew_game->setEnabled(true);
                    break;
                case E_NOTICE:
                    if(this->timerDialog)
                        this->timerDialog->accept();

                    emit logMessage("New event: E_NOTICE");
                    message = QString::fromStdString(this->worker->getClient()->getMessage());
                    QMessageBox::information(this, "Labyrinth2015 - Notice", message);
                    emit logMessage(message);
                    break;
                case E_ERROR:
                    if(this->timerDialog)
                        this->timerDialog->accept();

                    emit logMessage("New event: E_ERROR");
                    message = QString::fromStdString(this->worker->getClient()->getMessage());
                    QMessageBox::warning(this, "Labyrinth2015 - Error", message);
                    emit logMessage(message);
                    break;
                case E_TURN:
                    if(this->timerDialog)
                        this->timerDialog->accept();

                    emit logMessage("New event: E_TURN");
                    invitedBy = this->worker->getClient()->getInvitedBy();

                    this->desk->setDesk(this->worker->getClient()->getCurrentGame()->getSize(), this->worker->getClient()->getCurrentGame(), this->worker->getClient()->getCurrentClient());

                    this->resize(this->desk->size().width(), this->desk->size().height()+20);
                    this->setMaximumSize(this->desk->size().width(), this->desk->size().height()+20);
                    this->setMinimumSize(this->desk->size().width(), this->desk->size().height()+20);

                    QMessageBox::information(this, "Labyrinth2015 - Turn", "Player "+QString::number(invitedBy)+" turn");
                    break;
                case E_GAMES:
                    if(this->timerDialog)
                        this->timerDialog->accept();

                    emit logMessage("New event: E_GAMES");
                    games = this->worker->getClient()->getGames();

                    if(games.size() == 0){
                        QMessageBox::warning(this, "Labyrinth2015", "There is no game present on server");
                        emit logMessage("There is no game present on server");
                    }
                    else{
                        QDialogGames dialog(games, this);

                        if(dialog.exec() == QDialog::Accepted){
                            if(dialog.getGames().size() == 1){
                                QString gameName = dialog.getGames().at(0)->text();
                                emit sendMessage("LOADGAME;"+gameName+";");
                            }
                            else{
                                emit logMessage("You have to specify one game");
                                QMessageBox::warning(this, "Labyrinth2015 - Error loading game", "You have to specify one game");
                            }
                        }
                        else
                            emit logMessage("Game was not loaded");

                    }
                    break;
                case E_PUT:
                    emit logMessage("New event: E_PUTd");
                    this->desk->getDesk()->enableMove();
                case E_MOVE:
                    break;
                case E_WINNER:
                    invitedBy = this->worker->getClient()->getInvitedBy();
                    QMessageBox::information(this, "Labyrinth2015 - Winner", "Player "+QString::number(this->worker->getClient()->getInvitedBy())+" won the game");
                    //this->desk->getDesk()->

                    break;
                case E_NONE:
                case E_CLIENTCOUNT:
                default:
                    //emit logMessage("No new event");
                    break;
            }
        }
        this->timer->start(1000);
    }
    else
        emit logMessage("You are not connected to the server");
}

void QMain::handleMove(int x, int y, int i){
    if(this->worker){
        emit sendMessage("MOVE;"+QString::number(x)+","+QString::number(y)+","+QString::number(i)+";");
        emit logMessage("MOVE;"+QString::number(x)+","+QString::number(y)+","+QString::number(i)+";");
    }
    else
        emit logMessage("You are not connected to the server");
}

void QMain::handlePut(int x, int y, stoneAngle angle){
    if(this->worker){
        emit sendMessage("PUT;"+QString::number(x)+","+QString::number(y)+","+QString::number((int)angle)+";");
        emit logMessage("PUT;"+QString::number(x)+","+QString::number(y)+","+QString::number((int)angle)+";");
    }
    else
        emit logMessage("You are not connected to the server");
}
