#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) : QWidget(parent){

}

void MyWidget::newState(bool state){
    this->stateMem = state;
    std::cout << "hello world" << std::endl;
    update();
}

void MyWidget::paintEvent(QPaintEvent* event){

    QPainter painter(this);

    if(this->stateMem){
        painter.setPen(QColor(Qt::green));
        painter.setBrush(QBrush(Qt::green));
    }
    else{
        painter.setPen(QColor(Qt::red));
        painter.setBrush(QBrush(Qt::red));
    }
    QSize s = size();

    std::cout << "width: " << s.width() << " height: " << s.height() << std::endl;

    painter.drawEllipse(1, 1, s.width()-2, s.height()-2);
    QWidget::paintEvent(event);
}
