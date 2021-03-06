#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    container = new QX11EmbedContainer(ui->terminal);
    container->resize(500, 464);

    process = new QProcess(container);

    QString executable("xterm");
    QStringList arguments;
    arguments << "-into";
    arguments << QString::number(container->winId());
    arguments << "./2001Console";
    process->start(executable, arguments);
}

MainWindow::~MainWindow()
{
    delete ui;
}
