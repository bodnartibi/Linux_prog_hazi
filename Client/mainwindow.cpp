#include "mainwindow.h"

void MainWindow::bid_buttonclicked()
{
    int face, quan;
    bool OK;

    face = add_face_textedit->text().toInt(&OK);
    if(!OK)
    {
        status_label->setText("Wrong input");
        return;
    }
    quan = add_quan_textedit->text().toInt(&OK);
    if(!OK)
    {
        status_label->setText("Wrong input");
        return;
    }

    status_label->setText("Your bid sent");

    emit set_new_bid(quan,face);

    add_quan_label->hide();
    add_quan_textedit->hide();
    add_face_label->hide();
    add_face_textedit->hide();
    set_bid_button->hide();
    chal_button->hide();

    return;

}

void MainWindow::this_is_your_turn()
{
    status_label->setText("This is your turn!");

    add_quan_label->show();
    add_quan_textedit->show();
    add_face_label->show();
    add_face_textedit->show();
    set_bid_button->show();
    chal_button->show();

    return;
}

void MainWindow::new_bid(int quan, int face)
{
    bid_quan_label->setText("Actual quantity: " + QString::number(quan));
    bid_face_label->setText("Actual face: " + QString::number(face));
    repaint();
    return;
}

void MainWindow::new_dices(int* new_dices)
{
    QString str("Your dices: ");
    int d;
    for(int i; i < MAX_DICE_NUM; i++)
    {
        d = *(new_dices + i);
        if (d < 1)
            continue;
        str.append(QString::number(d) + " ");
    }
    dices->setText(str);
    repaint();
    return;
}

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
    dices = new QLabel("");
    game_input_layout = new QGridLayout();
    main_layout->addWidget(dices);
    main_layout->addLayout(game_input_layout);

    bid_quan_label = new QLabel("");
    game_input_layout->addWidget(bid_quan_label,0,0);

    bid_face_label = new QLabel("");
    game_input_layout->addWidget(bid_face_label,0,1);

    add_quan_label = new QLabel("Add your quantity: ");
    game_input_layout->addWidget(add_quan_label,1,0);
    add_quan_label->hide();

    add_quan_textedit = new QLineEdit();
    game_input_layout->addWidget(add_quan_textedit,1,1);
    add_quan_textedit->hide();

    add_face_label = new QLabel("Add your face: ");
    add_face_label->hide();
    game_input_layout->addWidget(add_face_label,2,0);

    add_face_textedit = new QLineEdit();
    game_input_layout->addWidget(add_face_textedit,2,1);
    add_face_textedit->hide();

    set_bid_button = new QPushButton("New Bid");
    game_input_layout->addWidget(set_bid_button,2,2);
    set_bid_button->hide();

    chal_button = new QPushButton("Challenge!");
    game_input_layout->addWidget(chal_button,3,2);
    chal_button->hide();

    connect(set_bid_button,SIGNAL(clicked()),this,SLOT(bid_buttonclicked()));
    connect(chal_button,SIGNAL(clicked()),proc_msg,SLOT(challenge()));

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
    emit set_name(name_textedit->text().toStdString().c_str());
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

    connect(proc_msg,SIGNAL(this_is_your_turn()),this,SLOT(this_is_your_turn()));
    connect(proc_msg,SIGNAL(new_bid(int,int)),this,SLOT(new_bid(int,int)));
    connect(proc_msg,SIGNAL(new_dices(int*)),this,SLOT(new_dices(int*)));

    connect(this,SIGNAL(set_name(const char*)),proc_msg,SLOT(set_name(const char*)));
    connect(this,SIGNAL(set_new_bid(int,int)),proc_msg,SLOT(set_new_bid(int,int)));



}

MainWindow::~MainWindow()
{

}
