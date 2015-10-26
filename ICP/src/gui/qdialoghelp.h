/**
  * @file qdialoghelp.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for QDialogHelp
  */

#ifndef QDIALOGHELP_H
#define QDIALOGHELP_H

#include <QDialog>

namespace Ui {
class QDialogHelp;
}

/**
  * @brief Class QDialogHelp for showing help window
  */
class QDialogHelp : public QDialog
{
    Q_OBJECT

public:
    explicit QDialogHelp(QWidget *parent = 0);
    ~QDialogHelp();

private:
    Ui::QDialogHelp *ui;
};

#endif // QDIALOGHELP_H
