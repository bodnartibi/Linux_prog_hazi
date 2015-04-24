#include "mainwindow.h"


// felulet ujraformalasa, ha mar elkezdődött a jaték
void MainWindow::enterGameState()
{
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

    //centralWidget->setLayout(main_layout);
    connect(ready_button,SIGNAL(clicked()),this,SLOT(enterGameState()));
}

MainWindow::~MainWindow()
{

}
