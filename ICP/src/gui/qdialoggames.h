/**
  * @file qdialoggames.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QDialogGames
  */

#ifndef QDIALOGGAMES_H
#define QDIALOGGAMES_H

#include <QDialog>
#include <vector>
#include <QList>
#include <QListWidgetItem>

using namespace std;

namespace Ui {
class QDialogGames;
}

/**
  * @brief Class QDialogGames for listing remote games
  */
class QDialogGames : public QDialog
{
    Q_OBJECT

public:
    explicit QDialogGames(vector<string>, QWidget *parent = 0);
    ~QDialogGames();
    QList<QListWidgetItem*> getGames();

private:
    Ui::QDialogGames *ui;
};

#endif // QDIALOGGAMES_H
