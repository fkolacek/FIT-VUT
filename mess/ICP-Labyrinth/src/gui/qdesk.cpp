/**
  * @file qdesk.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Implementation file for QDesk
  */

#include "qdesk.h"
#include "ui_qdesk.h"

QDesk::QDesk(int deskSize, Game* game, Client* client, QWidget *parent) : QWidget(parent), ui(new Ui::QDesk){
    ui->setupUi(this);

    this->game = game;
    this->client = client;

    if(!game || !client){
        return;
    }

    this->colors[0] = QColor(Qt::red);
    this->colors[1] = QColor(Qt::blue);
    this->colors[2] = QColor(Qt::green);
    this->colors[3] = QColor(Qt::yellow);

    bool enabled = (client->getUID() == game->getTurnUID());

    this->deskSize = deskSize;

    this->setAutoFillBackground(true);
    this->setPalette(QPalette(Qt::white));

    this->resize((this->deskSize + 3) * 50 + 220, (this->deskSize + 4) * 50);
    this->setMinimumSize((this->deskSize + 3) * 50 + 220, (this->deskSize + 4) * 50);
    this->setMaximumSize((this->deskSize + 3) * 50 + 220, (this->deskSize + 4) * 50);

    //Stones
    for(int y = 0; y < this->deskSize; y++){
        for(int x = 0; x < this->deskSize; x++){
            stoneType type = game->getDesk()->getStone(x+1, y+1)->getType();
            stoneAngle angle = game->getDesk()->getStone(x+1, y+1)->getAngle();
            stoneItem item = game->getDesk()->getStone(x+1, y+1)->getItem();

            this->stones[x][y] = new QStone(x+1, y+1, type, angle, item, this);
            connect(this->stones[x][y], SIGNAL(opMove(int, int)), this, SLOT(opMove(int,int)));
            this->stones[x][y]->move((x+1)*50, ((this->deskSize+1)*50) - (y+1)*50);
            //this->stones[x][y]->setText(QString::number(x+1)+"x"+QString::number(y+1));

            this->stones[x][y]->setEnabled(false);
        }
    }

    QCustomButton* button;
    QLabel* label;
    //Buttons & labels
    for(int y = 0; y < this->deskSize; y++){
        for(int x = 0; x < this->deskSize; x++){

            //Buttons
            if(y == 0 && x%2 == 0 && x > 1){
                button = new QCustomButton(x, y+1, this);
                connect(button, SIGNAL(opPut(int,int)), this, SLOT(opPut(int,int)));
                button->move(x*50, (this->deskSize+1)*50);
                //button->setText(QString::number(x)+"x"+QString::number(y+1));
                this->buttons.push_back(button);
                button->setEnabled(enabled);
            }
            if(y == this->deskSize-1 && x%2 == 0 && x > 1){
                button = new QCustomButton(x, y+1, this);
                connect(button, SIGNAL(opPut(int,int)), this, SLOT(opPut(int,int)));
                button->move(x*50, 0);
                //button->setText(QString::number(x)+"x"+QString::number(y+1));
                this->buttons.push_back(button);
                button->setEnabled(enabled);
            }
            if(x == 0 && y%2 == 0 && y > 1){
                button = new QCustomButton(x+1, y, this);
                connect(button, SIGNAL(opPut(int,int)), this, SLOT(opPut(int,int)));
                button->move(0, (this->deskSize+1)*50 - y*50);
                //button->setText(QString::number(x+1)+"x"+QString::number(y));
                this->buttons.push_back(button);
                button->setEnabled(enabled);
            }

            if(x == this->deskSize-1 && y%2 == 0 && y > 1){
                button = new QCustomButton(x+1, y, this);
                connect(button, SIGNAL(opPut(int,int)), this, SLOT(opPut(int,int)));
                button->move((this->deskSize+1)*50, (this->deskSize+1)*50 - y*50);
                //button->setText("c"+QString::number(x+1)+"x"+QString::number(y));
                this->buttons.push_back(button);
                button->setEnabled(enabled);
            }



            //Labels
            if(y == this->deskSize - 1){
                label = new QLabel(this);
                label->resize(50, 50);
                label->setText(QString::number(x+1));
                label->setAlignment(Qt::AlignCenter);
                label->setStyleSheet("font-weight: bold;");
                label->move((x+1)* 50, (y+3)*50);

            }
            if(x == this->deskSize - 1){
                label = new QLabel(this);
                label->resize(50, 50);
                label->setText(QString::number(this->deskSize - y));
                label->setAlignment(Qt::AlignCenter);
                label->setStyleSheet("font-weight: bold;");
                label->move((x+3)* 50, (y+1)*50);

            }

            this->labels.push_back(label);
        }
    }

    QFont font;
    font.setFamily("Waree");
    font.setPointSize(11);
    font.setBold(true);
    font.setWeight(60);

    //Info
    QPalette pal;

    pal.setColor(QPalette::WindowText, this->colors[(this->game->findPlayerByUID(client->getUID()))]);

    QLabel* playerInfo = new QLabel(this);
    playerInfo->resize(180, 20);
    playerInfo->setText("Player:\t\t"+QString::number(client->getUID()));
    playerInfo->move((this->deskSize + 3) * 50 + 20, 20);
    playerInfo->setFont(font);
    playerInfo->setPalette(pal);

    pal.setColor(QPalette::WindowText, this->colors[(this->game->getTurn())]);


    QLabel* playerTurn = new QLabel(this);
    playerTurn->resize(180, 20);
    playerTurn->setText("Turn:\t\t"+QString::number(game->getTurnUID()));
    playerTurn->move((this->deskSize + 3) * 50 + 20, 40);
    playerTurn->setFont(font);
    playerTurn->setPalette(pal);

    QLabel* playerCards = new QLabel(this);
    playerCards->resize(180, 20);
    playerCards->setText("Cards:\t\t"+QString::number(game->getDesk()->getCards()));
    playerCards->move((this->deskSize + 3) * 50 + 20, 60);
    playerCards->setFont(font);

    QLabel* playerCard = new QLabel(this);
    playerCard->resize(180, 20);
    stoneItem card = game->getPlayerCard(game->findPlayerByUID(client->getUID()));
    playerCard->setText("Current card:\t"+QString((char)((int)card + 'A')));
    playerCard->move((this->deskSize + 3) * 50 + 20, 90);
    playerCard->setFont(font);

    QLabel* playerStone = new QLabel(this);
    playerStone->resize(180, 20);
    playerStone->setText("Current stone:");
    playerStone->move((this->deskSize + 3) * 50 + 20, 110);
    playerStone->setFont(font);

    Stone* cs = game->getDesk()->getCurrentStone();
    this->currentStone = new QStone(0, 0, cs->getType(), cs->getAngle(), cs->getItem(), this);
    this->currentStone->move((this->deskSize + 3) * 50 + 70, 140);
    this->currentStone->setEnabled(enabled);

    this->btnLeft = new QPushButton(this);
    this->btnRight = new QPushButton(this);

    this->btnLeft->setText("Left");
    this->btnRight->setText("Right");

    this->btnLeft->resize(50, 50);
    this->btnRight->resize(50, 50);

    this->btnLeft->move((this->deskSize + 3) * 50 + 15, 140);
    this->btnRight->move((this->deskSize + 3) * 50 + 125, 140);

    this->btnLeft->setEnabled(enabled);
    this->btnRight->setEnabled(enabled);

    connect(this->btnLeft, SIGNAL(clicked(bool)), this, SLOT(rotateLeft()));
    connect(this->btnRight, SIGNAL(clicked(bool)), this, SLOT(rotateRight()));

    QLabel* figure;
    QLabel* playerScores;
    for(int z = 0; z < this->game->getPlayersCount(); z++){
        figure = new QLabel(this);
        figure->resize(10, 10);
        figure->setAutoFillBackground(true);
        figure->setPalette(QPalette(this->colors[z]));
        Position pos = this->game->getDesk()->getFigurePosition(z);

        figure->move((pos.x)*50 + 8, (this->deskSize +1 - pos.y)*50 + 8);
        figure->show();

        QPalette pal;
        pal.setColor(QPalette::WindowText, this->colors[z]);

        playerScores = new QLabel(this);
        playerScores->resize(180, 20);
        playerScores->setText("Player "+QString::number(this->game->getUID(z))+":\t"+QString::number(this->game->getPlayerScore(z)));
        playerScores->setPalette(pal);

        playerScores->move((this->deskSize + 3) * 50 + 20, 210 + z*20);
        playerScores->setFont(font);

    }


}

QDesk::~QDesk(){
    delete ui;
/*
    for(int y = 0; y < this->deskSize; y++){
        for(int x = 0; x < this->deskSize; x++){
            if(this->stones[x][y])
                delete this->stones[x][y];

            this->stones[x][y] = NULL;
        }
    }

    for(QVector<QCustomButton*>::iterator p = this->buttons.begin(); p != this->buttons.end(); p++){
        if(*p)
            delete *p;

        *p = NULL;
    }

    if(this->currentCard)
        delete this->currentCard;

    this->currentCard = NULL;

    if(this->currentStone)
        delete this->currentStone;

    this->currentStone = NULL;
*/
}

void QDesk::rotateLeft(){
    if(this->game){
        this->game->rotateLeft();
        Stone* cs = game->getDesk()->getCurrentStone();
        this->currentStone->refresh(cs->getType(), cs->getAngle(), cs->getItem());
    }

}
void QDesk::rotateRight(){
    if(this->game){
        this->game->rotateRight();
        Stone* cs = game->getDesk()->getCurrentStone();
        this->currentStone->refresh(cs->getType(), cs->getAngle(), cs->getItem());
    }
}

void QDesk::opMove(int x, int y){
     emit fireMove(x, y, this->game->findPlayerByUID(this->client->getUID()));

/*
    QMessageBox::warning(this, "Labyrinth2015 - Move", QString::number(x)+":"+QString::number(y));

    try{
        this->game->move(x, y, this->game->findPlayerByUID(this->client->getUID()));

        for(int a = 0; a < this->deskSize; a++){
            for(int b = 0; b < this->deskSize; b++){
                this->stones[b][a]->setEnabled(false);
            }
        }


    }
    catch(Exception* e){
        QMessageBox::warning(this, "Labyrinth2015 - Exception", QString::fromStdString(e->getMessage()));
    }
*/
}
void QDesk::opPut(int x, int y){
    emit firePut(x, y, this->game->getDesk()->getCurrentStone()->getAngle());
}

void QDesk::enableMove(){
    if(client->getUID() != game->getTurnUID())
        return;

    Stone* stone = game->getDesk()->getCurrentStone();
    this->currentStone->refresh(stone->getType(), stone->getAngle(), stone->getItem());
    this->currentStone->setEnabled(false);

    for(QVector<QCustomButton*>::iterator p = this->buttons.begin(); p != this->buttons.end(); p++)
        (*p)->setEnabled(false);

    this->btnLeft->setEnabled(false);
    this->btnRight->setEnabled(false);

    for(int a = 0; a < this->deskSize; a++){
        for(int b = 0; b < this->deskSize; b++){
            stone = this->game->getDesk()->getStone(b+1, a+1);
            this->stones[b][a]->refresh(stone->getType(), stone->getAngle(), stone->getItem());
            this->stones[b][a]->setEnabled(true);
        }
    }
}
