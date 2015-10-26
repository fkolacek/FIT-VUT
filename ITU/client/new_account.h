#ifndef NEW_ACCOUNT_H
#define NEW_ACCOUNT_H

#include <QDialog>
#include <QSettings>
#include <QFile>
#include <QTextStream>

namespace Ui {
class New_account;
}

class New_account : public QDialog
{
    Q_OBJECT
    
public:
    explicit New_account(QWidget *parent = 0);
    ~New_account();
    
private slots:
    void on_cancel_button_clicked();

    void on_create_new_acc_clicked();

private:
    Ui::New_account *ui;
    QString m_sSettingsFile;
};

#endif // NEW_ACCOUNT_H
