#include "mainwindow.h"
#include "insertwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QtGui>
#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow main;
    main.show();

    return a.exec();
}
