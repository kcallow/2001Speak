#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::readText(){
    //leer de consola
//    QTextStream qtin(stdin);
    std::string line;
    //while(true) {
      //  QString line = qtin.readLine();
        std::cin >> line;
        setWindowText((line));
   // }
}
*/

void MainWindow::setWindowText(std::string text) {
        ui->textBrowser->setText(QString::fromStdString(text));
}
