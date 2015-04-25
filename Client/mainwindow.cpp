#include "mainwindow.h"


void MainWindow::getMessage()
{
    QByteArray buff;
    int length;
    char* buff_p;
    buff = socket->readAll();
    buff_p = buff.data();
    length = buff.length();

    emit process_msg(buff_p, length);
    return;
}

void MainWindow::sendMessage(void* msg, int msglen)
{
    socket->write((char*)msg,msglen);
    socket->flush();
    return;
}

void MainWindow::enterGameState()
{
    status_label->setText("Game started, waiting for dices");
    dices = new QLabel("Your dices: ");
    game_input_layout = new QGridLayout();
    main_layout->addWidget(dices);
    main_layout->addLayout(game_input_layout);

    bid_quan_label = new QLabel("Actual quantity: ");
    game_input_layout->addWidget(bid_quan_label,0,0);

    bid_face_label = new QLabel("Actaula bid: ");
    game_input_layout->addWidget(bid_face_label,0,1);

    add_quan_label = new QLabel("Add your quantity: ");
    game_input_layout->addWidget(add_quan_label,1,0);

    add_quan_textedit = new QLineEdit();
    game_input_layout->addWidget(add_quan_textedit,1,1);

    add_face_label = new QLabel("Add your face: ");
    game_input_layout->addWidget(add_face_label,2,0);

    add_face_textedit = new QLineEdit();
    game_input_layout->addWidget(add_face_textedit,2,1);

    set_bid_button = new QPushButton("New Bid");
    game_input_layout->addWidget(set_bid_button,2,2);

    chal_button = new QPushButton("Challenge!");
    game_input_layout->addWidget(chal_button,3,2);


    proc_msg->my_name.append(name_textedit->text());

    repaint();
    return;
}


void MainWindow::connectionReady()
{
    isConnected = true;
    status_label->setText("Connected");
    repaint();
    if(isReady)
        enterGameState();
}

// felulet ujraformalasa, ha mar elkezdődött a jaték
void MainWindow::nameIsSet()
{
    isReady = true;
    name_label->setText("Your name: " + name_textedit->text());
    name_textedit->hide();
    ready_button->hide();
    repaint();
    if(isConnected)
        enterGameState();
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
    connect(socket,SIGNAL(readyRead()),this,SLOT(getMessage()));
    socket->connectToHost("localhost",5000);

    proc_msg = new ClientMessages;
    connect(proc_msg,SIGNAL(send_msg(void*,int)),this,SLOT(sendMessage(void*,int)));
    connect(this,SIGNAL(process_msg(void*,int)),proc_msg,SLOT(process_client_message(void*,int)));
}

MainWindow::~MainWindow()
{

}
