/**
  * @file qdialoginvite.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QDialogInvite
  */
  
#ifndef QDIALOGINVITE_H
#define QDIALOGINVITE_H

#include <QDialog>
#include <vector>
#include <QStringList>
#include "../cli/client.h"

using namespace std;

namespace Ui {
class QDialogInvite;
}

/**
  * @brief Class QDialogInvite used to invite other players
  */
class QDialogInvite : public QDialog
{
    Q_OBJECT

public:
    explicit QDialogInvite(vector<Client> clients, Client*, QWidget *parent = 0);
    QStringList getPlayers();
    ~QDialogInvite();


private:
    Ui::QDialogInvite *ui;
};

#endif // QDIALOGINVITE_H
