#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <iostream>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);
    
public slots:
     void newState(bool);
protected:
     bool stateMem;
     void paintEvent(QPaintEvent*);
    
};

#endif // MYWIDGET_H
