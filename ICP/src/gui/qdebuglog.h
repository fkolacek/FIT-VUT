/**
  * @file qdebuglog.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QDebugLog
  */

#ifndef QDEBUGLOG_H
#define QDEBUGLOG_H

#include <QDialog>

namespace Ui {
class QDebugLog;
}

/**
  * @brief Class QDebugLog used for debugging
  */
class QDebugLog : public QDialog
{
    Q_OBJECT

public:
    explicit QDebugLog(QWidget *parent = 0);
    ~QDebugLog();
public slots:
    void receiveMessage(QString);

private:
    Ui::QDebugLog *ui;
};

#endif // QDEBUGLOG_H
