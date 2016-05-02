/**
  * @file qdesk.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QDesk
  */

#ifndef QDESK_H
#define QDESK_H

#include <QWidget>
#include <QVector>
#include <QMessageBox>
#include <QLabel>
#include <QColor>
#include <QTextBrowser>
#include <QString>
#include "qstone.h"
#include "qcustombutton.h"
#include "../cli/game.h"
#include "../cli/utils.h"

namespace Ui {
class QDesk;
}

/**
  * @brief Class QDesk represents game desk
  */
class QDesk : public QWidget{
    Q_OBJECT

public:
    explicit QDesk(int, Game*, Client*, QWidget *parent = 0);
    ~QDesk();

public slots:
    void rotateLeft();
    void rotateRight();

    void opMove(int, int);
    void opPut(int, int);

    void enableMove();
signals:
    void fireMove(int, int, int);
    void firePut(int, int, stoneAngle);

private:
    int deskSize;
    QStone* stones[11][11];
    QVector<QCustomButton*> buttons;
    QVector<QLabel*> labels;

    QStone* currentStone;
    QStone* currentCard;
    QPushButton* btnLeft;
    QPushButton* btnRight;
    QColor colors[4];

    Game* game;
    Client* client;

    Ui::QDesk *ui;
};

#endif // QDESK_H
