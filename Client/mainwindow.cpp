#include "mainwindow.h"


void MainWindow::server_disconnected()
{
    QDialog  diag;
    QVBoxLayout lay;
    diag.setLayout(&lay);
    lay.addWidget(new QLabel("\nFatal: Server disconnected\n"));
    QPushButton btn;
    btn.setText("OK");
    lay.addWidget(&btn);
    connect(&btn,SIGNAL(clicked()),&diag,SLOT(close()));
    diag.exec();
    exit(0);
}

void MainWindow::sy_won(const char* name, bool is_it_you)
{
    ready_button->show();
    QString str(name);
    if(is_it_you)
    {
        info_window-> append("YOU WON");
        status_label->setText("YOU WON");
    }
    else
    {
        QString str(name);
        info_window->append("You failed, " + str + " won");
        status_label->setText("You failed, " + str + " won");
    }

    add_quan_label->hide();
    add_quan_textedit->hide();
    add_face_label->hide();
    add_face_textedit->hide();
    set_bid_button->hide();
    chal_button->hide();

    dices->setText("");
    bid_face_label->setText("");
    bid_quan_label->setText("");

    repaint();
}



void MainWindow::sy_disconnected(const char* name)
{
    ready_button->show();
    QString str(name);
    info_window-> append(str + " disconnected, please send \"ready\" again");
    add_quan_label->hide();
    add_quan_textedit->hide();
    add_face_label->hide();
    add_face_textedit->hide();
    set_bid_button->hide();
    chal_button->hide();

    dices->setText("");
    bid_face_label->setText("");
    bid_quan_label->setText("");

    repaint();
}

void MainWindow::new_info_msg(const char* msg)
{

    QString str(msg);
    str.replace("\n","");
    info_window-> append(str);
    return;
}

void MainWindow::ready_clicked()
{
    ready_button->hide();
    emit client_is_ready();
}

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

    status_label->setText("Waiting for your turn");

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

    info_window->append("This is your turn!");
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
    status_label->setText("New turn");
    QString str("Your dices: ");
    int d;
    for(int i = 0; i < MAX_DICE_NUM; i++)
    {
        d = *(new_dices + i);
        if (d < 1)
            continue;
        str.append(QString::number(d) + " ");
    }
    dices->setText(str);
    add_quan_label->hide();
    add_quan_textedit->hide();
    add_face_label->hide();
    add_face_textedit->hide();
    set_bid_button->hide();
    chal_button->hide();
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
    status_label->setText("Waiting for other clients");


    repaint();
    return;
}


void MainWindow::connectionReady()
{
    isConnected = true;
    status_label->setText("Connected");
    name_button->setText("Set name");
    repaint();
    if(isReady)
        enterGameState();
}

// felulet ujraformalasa, ha mar elkezdődött a jaték
void MainWindow::nameIsSet()
{
    if(!isConnected){
        status_label->setText("Cannot find the server, try again");
        bool ok;
        QString addr = QInputDialog::getText(this,
          "Server address", "Please enter the server host:",
          QLineEdit::Normal, "localhost", &ok);
        if(ok && !addr.isEmpty())
        {
            socket->connectToHost(addr,5000);
        }
        return;
    }

    isReady = true;
    name_label->setText("Your name: " + name_textedit->text());
    name_textedit->hide();
    name_button->hide();
    ready_button->show();
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

    centralWidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    // TODO felszabaditasok?
    main_layout = new QVBoxLayout(centralWidget);
    this->setWindowTitle("Liar's dice");

    name_layout = new QHBoxLayout();


    name_label = new QLabel("Please add your name: ");
    name_layout->addWidget(name_label);

    name_textedit = new QLineEdit();
    name_layout->addWidget(name_textedit);

    name_button = new QPushButton("Connect");

    main_layout->addLayout(name_layout);
    name_layout->addWidget(name_button);

    ready_button = new QPushButton("Ready");

    name_layout->addWidget(ready_button);
    ready_button->hide();

    ////
    QFont font;
    font.setPointSize(16);
    status_label = new QLabel("Connecting to server...");
    main_layout->addWidget(status_label);
    status_label->setFont(font);

    ////
    info_window = new QTextEdit();
    info_window->setReadOnly(true);

    //sb = info_window->verticalScrollBar();
    //sb->setValue(sb->maximum());

    main_layout->addWidget(info_window);

    connect(name_button,SIGNAL(clicked()),this,SLOT(nameIsSet()));
    connect(ready_button,SIGNAL(clicked()),this,SLOT(ready_clicked()));

    proc_msg = new ClientMessages;
    connect(this,SIGNAL(client_is_ready()),proc_msg,SLOT(client_is_ready()));
    connect(proc_msg,SIGNAL(new_info_msg(const char*)),this,SLOT(new_info_msg(const char*)));

    socket = new QTcpSocket(this);

    connect(socket,SIGNAL(connected()),this,SLOT(connectionReady()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(getMessage()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(server_disconnected()));
    socket->connectToHost("localhost",5000);

    connect(proc_msg,SIGNAL(send_msg(void*,int)),this,SLOT(sendMessage(void*,int)));
    connect(this,SIGNAL(process_msg(void*,int)),proc_msg,SLOT(process_client_message(void*,int)));

    connect(proc_msg,SIGNAL(this_is_your_turn()),this,SLOT(this_is_your_turn()));
    connect(proc_msg,SIGNAL(new_bid(int,int)),this,SLOT(new_bid(int,int)));
    connect(proc_msg,SIGNAL(new_dices(int*)),this,SLOT(new_dices(int*)));

    connect(this,SIGNAL(set_name(const char*)),proc_msg,SLOT(set_name(const char*)));
    connect(this,SIGNAL(set_new_bid(int,int)),proc_msg,SLOT(set_new_bid(int,int)));
    connect(proc_msg,SIGNAL(sy_disconnected(const char*)),this,SLOT(sy_disconnected(const char*)));

    connect(proc_msg,SIGNAL(sy_won(const char*,bool)),this,SLOT(sy_won(const char*,bool)));

    ////

    dices = new QLabel("");
    dices->setFont(font);
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
    game_input_layout->addWidget(set_bid_button,1,2);
    set_bid_button->hide();

    chal_button = new QPushButton("Challenge!");
    game_input_layout->addWidget(chal_button,2,2);
    chal_button->hide();

    connect(set_bid_button,SIGNAL(clicked()),this,SLOT(bid_buttonclicked()));
    connect(chal_button,SIGNAL(clicked()),proc_msg,SLOT(challenge()));

}

MainWindow::~MainWindow()
{

}
