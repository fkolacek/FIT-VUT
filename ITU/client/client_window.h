#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QtNetwork/QTcpSocket>
#include <QListWidgetItem>
#include <QHash>
#include <iostream>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QSettings>
#include <QChar>
#include <QStringList>
#include <QDateTime>
#include <QIcon>

namespace Ui {
class Client_Window;
}

class Client_Window : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Client_Window(QWidget *parent = 0);
    ~Client_Window();
    
private slots:
    void on_actionAbout_triggered();

    void on_actionHelp_triggered();

    void on_actionNew_serv_triggered();

    void on_actionClose_serv_triggered();

    void on_actionDelete_acc_triggered();

    void on_actionPart_chan_triggered();

    void on_actionJoin_chan_triggered();

    void on_actionNew_acc_triggered();

    void on_actionChange_acc_triggered();

    void connectToServer(QString);
    void loadSetting();
    void connectEstablish();
    void readData();
    void saveToLog();
    void connectToChannel(QString);
    void actualizationChannel();

    void on_send_button_pressed();

    void on_chat_active_itemClicked(QListWidgetItem *item);

    void on_chat_input_returnPressed();

    void on_chat_nicks_itemDoubleClicked(QListWidgetItem *item);

    void on_smiles_button_clicked();

    void on_smile_00_clicked();

    void on_smile_01_clicked();

    void on_smile_02_clicked();

    void on_smile_03_clicked();

    void on_smile_04_clicked();

    void on_smile_10_clicked();

    void on_smile_11_clicked();

    void on_smile_12_clicked();

    void on_smile_13_clicked();

    void on_smile_14_clicked();

    void on_smile_20_clicked();

    void on_smile_21_clicked();

    void on_smile_22_clicked();

    void on_smile_23_clicked();

    void on_smile_24_clicked();

    void on_smile_30_clicked();

    void on_smile_31_clicked();

    void on_smile_32_clicked();

    void on_smile_33_clicked();

    void on_smile_34_clicked();

    void on_smile_40_clicked();

    void on_smile_41_clicked();

    void on_smile_42_clicked();

    void on_smile_43_clicked();

    void on_smile_44_clicked();

    void on_chat_nicks_itemClicked(QListWidgetItem *item);

    void on_actionExit_triggered();

private:
    void writeLine(const QString& line);
    void parseLine(QString line);
    void writeLog(QString filename, QString text);
    Ui::Client_Window *ui;
    QString serverAddres;
    int port;
    QString nick;
    QString password;
    QTcpSocket *socket;
    QString name;

    QString activChannel;
    QString serverName;

    QHash<QString, QString> chatText;
    bool amIConnected;
    QDir dir;

    QString m_sSettingsFile;

    QTime time;

signals:
    void Connected();

};

#endif // CLIENT_WINDOW_H
