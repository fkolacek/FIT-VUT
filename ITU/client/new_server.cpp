#include "new_server.h"
#include "ui_new_server.h"
#include <iostream>

New_Server::New_Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::New_Server)
{
    ui->setupUi(this);
    //m_sSettingsFile = QApplication::applicationDirPath() + "/init.ini";
    loadSettings();
    //qDebug() << QApplication::applicationDirPath() + ":/init.ini";
    QFile file("nicks.txt");
    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString nick = stream.readLine();
            ui->profile_pick->addItem(nick);
        }
        file.close();
    }
}

New_Server::~New_Server()
{
    delete ui;
}

void New_Server::on_cancel_button_clicked()
{
    this->close();
}

void New_Server::on_connect_button_clicked()
{
    //qDebug() << "buton click connect";
    QFile file("nicks.txt");
    bool isAlreadyThere = false;
    if (!file.exists()) {
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        stream << ui->profile_pick->currentText() << endl;
        file.close();
    }
    else {
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString nick = stream.readLine();
            if (nick.compare(ui->profile_pick->currentText()) == 0) {
                isAlreadyThere = true;
                break;
            }
        }
        file.close();
        if (!isAlreadyThere) {
            file.open(QIODevice::Append);
            QTextStream stream(&file);
            stream << ui->profile_pick->currentText() << endl;
            file.close();
        }
    }

    saveSettings();
    emit(newServer(ui->profile_pass->text()));
    // connect na dany server na port s nickem xy
    this->close();
}

void New_Server::loadSettings()
{
    //qDebug() << "load";
    QSettings settings(QApplication::applicationDirPath(), "init.ini");
    QString sText = settings.value("Server", "").toString();
    ui->serv_addr->setText(sText);
    QString port = settings.value("Port", "").toString();
    ui->port_num->setText(port);
    QString accText = settings.value("Account", "").toString();
    ui->profile_pick->setEditText(accText);
}

void New_Server::saveSettings()
{
    //qDebug() << "save";
    QSettings settings(QApplication::applicationDirPath(), "init.ini");
    //settings.beginGroup("TAG1");
    QString sText = (ui->serv_addr) ? ui->serv_addr->text() : "";
    settings.setValue("Server", sText);

    sText = (ui->port_num) ? ui->port_num->text() : "";
    settings.setValue("Port", sText);

    sText = (ui->profile_pick) ? ui->profile_pick->currentText() : "";
    settings.setValue("Account", sText);

}
