/**
  * @file qdialogloding.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QDialogLoading
  */

#ifndef QDIALOGLOADING_H
#define QDIALOGLOADING_H

#include <QDialog>

namespace Ui {
class QDialogLoading;
}

/**
  * @brief Class QDialogLoading - used as progressbar
  */
class QDialogLoading : public QDialog
{
    Q_OBJECT

public:
    explicit QDialogLoading(QWidget *parent = 0);
    ~QDialogLoading();

private:
    Ui::QDialogLoading *ui;
};

#endif // QDIALOGLOADING_H
