#include "mainwindow.h"

void MainWindow::connectionReady()
{
    isConnected = true;
}

// felulet ujraformalasa, ha mar elkezdődött a jaték
void MainWindow::nameIsSet()
{
    isReady = true;
    name_label->setText("Your name: " + name_textedit->text());
    name_textedit->hide();
    ready_button->hide();



    repaint();
    return;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //centralWidget = new QWidget(this);
    //this->setCentralWidget(this);

    centralWidget = new QWidget(this);
    this->setCentralWidget( centralWidget );

    // TODO felszabaditasok?
    main_layout = new QVBoxLayout(centralWidget);
    this->setWindowTitle("Liar's dice");

    name_layout = new QHBoxLayout();


    name_label = new QLabel("Please add your name: ");
    name_layout->addWidget(name_label);

    name_textedit = new QLineEdit();
    name_layout->addWidget(name_textedit);

    ready_button = new QPushButton("Ready");

    main_layout->addLayout(name_layout);
    name_layout->addWidget(ready_button);

    status_label = new QLabel("Connecting to server...");
    main_layout->addWidget(status_label);

    connect(ready_button,SIGNAL(clicked()),this,SLOT(nameIsSet()));

    socket = new QTcpSocket(this);

    connect(socket,SIGNAL(connected()),this,SLOT(connectionReady()));
    socket->connectToHost("localhost",5000);
}

MainWindow::~MainWindow()
{

}
