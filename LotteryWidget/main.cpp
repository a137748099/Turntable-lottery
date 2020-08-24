#include "mainwindow.h"
#include <QApplication>
#include <QTimer>
#include <QtAndroid>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTimer::singleShot(1000,NULL,[=](){
        QtAndroid::hideSplashScreen(1000);
    });
    MainWindow w;
    w.show();

    return a.exec();
}
