#include "new_account.h"
#include "ui_new_account.h"

New_account::New_account(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::New_account)
{
    ui->setupUi(this);
}

New_account::~New_account()
{
    delete ui;
}

void New_account::on_cancel_button_clicked()
{
    this->close();
}

void New_account::on_create_new_acc_clicked()
{
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    QString sText = ui->nickname->text();
    settings.setValue("Account", sText);

    QFile file("nicks.txt");
    bool isAlreadyThere = false;
    if (!file.exists()) {
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        stream << sText << endl;
        file.close();
    }
    else {
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString nick = stream.readLine();
            if (nick.compare(sText) == 0) {
                isAlreadyThere = true;
                break;
            }
        }
        file.close();
        if (!isAlreadyThere) {
            file.open(QIODevice::Append);
            QTextStream stream(&file);
            stream << sText << endl;
            file.close();
        }
    }

    // and send this shit to server

    this->close();
}
