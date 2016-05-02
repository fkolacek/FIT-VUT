/**
  * @file qcustomwarpper.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QCustomWrapper
  */

#ifndef QCUSTOMWRAPPER_H
#define QCUSTOMWRAPPER_H

#include <QWidget>
#include <QLabel>
#include "qdesk.h"

/**
  * @brief Class QCustomWrapper used for wrapping QDesk objects
  */

class QCustomWrapper : public QWidget
{
    Q_OBJECT
public:
    QCustomWrapper(QWidget* parent = 0);
    QDesk* getDesk();
    void setDesk(int, Game*, Client*);
    void clearDesk();
    void showLabel(bool = true);

private:
    QWidget* p;
    QDesk* desk;
    QLabel* mainLabel;
};

#endif // QCUSTOMWRAPPER_H
