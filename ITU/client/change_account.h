#ifndef CHANGE_ACCOUNT_H
#define CHANGE_ACCOUNT_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QSettings>

namespace Ui {
class Change_account;
}

class Change_account : public QDialog
{
    Q_OBJECT
    
public:
    explicit Change_account(QWidget *parent = 0);
    ~Change_account();
    
private slots:
    void on_cancel_button_clicked();

    void on_change_acc_button_clicked();

private:
    Ui::Change_account *ui;
    QString m_sSettingsFile;
};

#endif // CHANGE_ACCOUNT_H
