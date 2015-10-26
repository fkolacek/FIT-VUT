#include "join_chan.h"
#include "ui_join_chan.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Join_chan::Join_chan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Join_chan)
{
    ui->setupUi(this);
    loadChannel();
}

Join_chan::~Join_chan()
{
    delete ui;
    loadChannel();
}

void Join_chan::on_cancel_button_clicked()
{
    this->close();
}

void Join_chan::on_join_button_clicked()
{
    // connect to selected channel
    QString channel = ui->chan_name->currentText();
    //qDebug() << channel.mid(0,1);
    if(channel != "")
    {
        if(channel.mid(0,1) != "#")
        {
            //qDebug() << "podminka";
            channel = QString("#%1").arg(channel);
        }
        saveChannel(channel);
        emit(newChannel(channel));
        this->close();
    }
}

void Join_chan::loadChannel()
{
    QFile file("channel.txt");

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull())
    {
        ui->chan_name->addItem(line);
        line = in.readLine();
    }

    file.close();
}

void Join_chan::saveChannel(QString channel)
{
    QFile file("channel.txt");

    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream out(&file);
    QString line = out.readLine();

    while (channel != line)
    {
        if(line.isNull())
        {
            out << channel << "\n";
            break;
        }

        line = out.readLine();
    }
    file.close();
}
