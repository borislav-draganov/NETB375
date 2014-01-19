#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "insertwindow.h"
#include "searchwindow.h"

#include <QtGui>
#include <QtSql>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    static QSqlDatabase connectDB();	// A static function to be called when an open connection to the database is needed

public slots:
    void insertNew();               // A slot for creating a new "Insert" window
    void close_insert();            // A slot for closing an "Insert" window
    void searchNew();               // A slot for creating a new "Search" window
    void close_search();            // A slot for closing a "Search" window

private:
    QPushButton *insertButton;      // The "Insert" button
    QPushButton *searchButton;      // The "Search" button
    insertWindow *new_item;         // An "Insert" window
    searchWindow *new_search;       // A "Search" window
};

#endif // MAINWINDOW_H
