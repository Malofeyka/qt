#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QPixmap>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();


    return a.exec();

}
