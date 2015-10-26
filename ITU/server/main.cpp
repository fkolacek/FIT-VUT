#include <QCoreApplication>
#include <QTextCodec>
#include "qircserver.h"

int main(int argc, char *argv[]){

    QCoreApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    app.setApplicationName("QIrcServer");
    app.setApplicationVersion("0.1");
    
    QIrcServer Forrest;

    //Run Forrest, run!
    Forrest.run();

    return app.exec();
}
