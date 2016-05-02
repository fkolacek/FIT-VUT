/**
  * @file qdialogconnect.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for dialogConnect
  */

#ifndef DIALOGCONNECT_H
#define DIALOGCONNECT_H

#include <QDialog>

namespace Ui {
class dialogConnect;
}

/**
  * @brief Class dialogConnect used for connecting to remote server
  */
class dialogConnect : public QDialog{
    Q_OBJECT

public:
    explicit dialogConnect(QWidget *parent = 0);
    ~dialogConnect();

    QString getHostname();
    int getPort();

private:
    Ui::dialogConnect *ui;
};

#endif // DIALOGCONNECT_H
