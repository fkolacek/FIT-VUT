/**
  * @file main.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Main function for GUI client
  */

#include "qmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMain w;
    w.show();

    return a.exec();
}
