#ifndef MAIN_FORM__H
#define MAIN_FORM__H

#include <QtCore>
#include <QtCore>
#include <QIntValidator>
#include "ui_untitled.h"

class MainForm : public QWidget {
  Q_OBJECT

  private:
    Ui::Form ui;
    QIntValidator* validator;

  public:
    MainForm ( QWidget * parent = 0, Qt::WindowFlags f = 0 );
    virtual ~MainForm();

public slots:
    void buttonPressed();
signals:
    void haveResult(QString);
    void setState(bool);


};

#endif // MAIN_FORM__H
