#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QDebug>
#include <QtGui>
#include <QLineEdit>

#include <QtNetwork/QTcpSocket>
#include <clientMessages.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ClientMessages* proc_msg;

private:

    QWidget *centralWidget;


    // fo layout
    QVBoxLayout* main_layout;

    // legfelso Layout
    // nev label es nev textedit
    QHBoxLayout* name_layout;
        QLabel* name_label;
        QLineEdit* name_textedit;
        QPushButton* name_button;
        QPushButton* ready_button;

    // status label
    QLabel* status_label;

    // jatek fo layout
    QVBoxLayout* game_main_layout;

        QLabel* dices;

        QGridLayout* game_input_layout;
        QLabel* bid_quan_label;
        QLabel* bid_face_label;
        QLabel* add_quan_label;
        QLineEdit* add_quan_textedit;
        QLabel* add_face_label;
        QLineEdit* add_face_textedit;
        QPushButton* set_bid_button;
        QPushButton* chal_button;

        QLabel* actual_bid_layout;

    QTcpSocket* socket;


    bool isReady = false;
    bool isConnected = false;

    void enterGameState();

public slots:
    void nameIsSet();
    void ready_clicked();
    void connectionReady();
    void getMessage();
    void sendMessage(void* msg, int msglen);

    void bid_buttonclicked();

    void this_is_your_turn();
    void new_bid(int quan, int face);
    void new_dices(int* new_dices);

signals:
    void process_msg(void* msg, int len);
    void set_name(const char* name);
    void set_new_bid(int quan, int face);
    void client_is_ready();

};

#endif // MAINWINDOW_H
