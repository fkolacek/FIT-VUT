#ifndef NEW_SERVER_H
#define NEW_SERVER_H

#include <QDialog>
#include <QSettings>
#include <QFile>
#include <QTextStream>

namespace Ui {
class New_Server;
}

class New_Server : public QDialog
{
    Q_OBJECT
    
public:
    explicit New_Server(QWidget *parent = 0);
    ~New_Server();

private:
    void loadSettings();
    void saveSettings();

private slots:
    void on_cancel_button_clicked();

    void on_connect_button_clicked();

private:
    Ui::New_Server *ui;
    QString m_sSettingsFile;

signals:
    void newServer(QString);
};

#endif // NEW_SERVER_H
