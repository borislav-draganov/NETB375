#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtSql>
#include "insertwindow.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    static QSqlDatabase connectDB();

    //~MainWindow();

public slots:
    void insertNew();               // A slot for creating a new "Insert" window
    void close_insert();            // A slot for closing an "Insert" window

private:
    QPushButton *insertButton;      // The "Insert" button
    QPushButton *searchButton;      // The "Search" button
    insertWindow *new_item;         // An "Insert" window
};

#endif // MAINWINDOW_H
