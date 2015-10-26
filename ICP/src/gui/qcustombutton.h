/**
  * @file cli.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QCustomButton
  */

#ifndef QCUSTOMBUTTON_H
#define QCUSTOMBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QIcon>

/**
  * @brief Class QCustomButton
  */

class QCustomButton : public QPushButton{
    Q_OBJECT

    public:
        QCustomButton(int, int, QWidget* = 0);
        int getX();
        int getY();
    public slots:
        void handleClicked(bool checked);
    signals:
        void opPut(int, int);

    private:
        int x;
        int y;
};

#endif // QCUSTOMBUTTON_H
