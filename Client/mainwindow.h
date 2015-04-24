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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    QWidget *centralWidget;

    // fo layout
    QVBoxLayout* main_layout;

    // legfelso Layout
    // nev label es nev textedit
    QHBoxLayout* name_layout;
        QLabel* name_label;
        QLineEdit* name_textedit;
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

    QTcpSocket* socket;

public slots:
    void enterGameState();

signals:


};

#endif // MAINWINDOW_H
