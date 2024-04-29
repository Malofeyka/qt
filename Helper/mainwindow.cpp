#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QPixmap>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

}
