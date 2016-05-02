/**
  * @file qdialognewgame.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QDialogNewGame
  */

#ifndef QDIALOGNEWGAME_H
#define QDIALOGNEWGAME_H

#include <QDialog>

namespace Ui {
class QDialogNewGame;
}

/**
  * @brief Class QDialogNewGame for creating new game
  */
class QDialogNewGame : public QDialog
{
    Q_OBJECT

public:
    explicit QDialogNewGame(QWidget *parent = 0);
    ~QDialogNewGame();

    int getPlayers();
    int getCards();
    int getSize();

private:
    Ui::QDialogNewGame *ui;
};

#endif // QDIALOGNEWGAME_H
