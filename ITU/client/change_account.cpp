#include "change_account.h"
#include "ui_change_account.h"

Change_account::Change_account(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Change_account)
{
    ui->setupUi(this);
    QFile file("nicks.txt");
    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString nick = stream.readLine();
            ui->profile_pick->addItem(nick);
        }
        file.close();
        if (ui->profile_pick->currentText().isEmpty())
            ui->change_acc_button->setEnabled(false);
    }
    else
        ui->change_acc_button->setEnabled(false);
}

Change_account::~Change_account()
{
    delete ui;
}

void Change_account::on_cancel_button_clicked()
{
    this->close();
}

void Change_account::on_change_acc_button_clicked()
{
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    QString sText = ui->profile_pick->currentText();
    settings.setValue("Account", sText);

    this->close();
}
