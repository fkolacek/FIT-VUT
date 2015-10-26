#ifndef JOIN_CHAN_H
#define JOIN_CHAN_H

#include <QDialog>

namespace Ui {
class Join_chan;
}

class Join_chan : public QDialog
{
    Q_OBJECT
    
public:
    explicit Join_chan(QWidget *parent = 0);
    ~Join_chan();
    
private slots:
    void on_cancel_button_clicked();

    void on_join_button_clicked();

    void loadChannel();
    void saveChannel(QString);

private:
    Ui::Join_chan *ui;

signals:
    void newChannel(QString);
};

#endif // JOIN_CHAN_H
