#include "mainwindow.h"
#include "insertwindow.h"

#include <QGridLayout>
#include <QMessageBox>
#include <stdexcept>

// Constructor
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent){

    // Create and set a grid layout
    QGridLayout *mainLayout = new QGridLayout(this);
    setLayout(mainLayout);
    resize(200, 300);

    // Instantiate the buttons
    insertButton = new QPushButton(tr("&Insert"));
    insertButton->show();
    mainLayout->addWidget(insertButton, 0, 0);

    searchButton = new QPushButton(tr("&Search"));
    searchButton->show();
    mainLayout->addWidget(searchButton, 1, 0);

    // Connect to the slots
    connect(insertButton, SIGNAL(clicked()), this, SLOT(insertNew()));

    // Nullify the pointer
    new_item = NULL;
}

// Opens a new window to insert new data in the database
void MainWindow::insertNew() {
    // Open a window only if there isn't one already
    if(!new_item) {
        new_item = new insertWindow;
        new_item->show();

        // Connect to the closing event of that window
        connect(new_item, SIGNAL(closedSignal()), this, SLOT(close_insert()));
    }
}

// Close the "Insert" window
void MainWindow::close_insert() {
    delete new_item;
    new_item = NULL;
}

QSqlDatabase MainWindow::connectDB() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    QFile file("config.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        throw QString("Could not open DB config file!");
    }

    QTextStream config_file(&file);

    QString line;
    QStringList data;

    // Set Host Name
    line = config_file.readLine();
    data = line.split(" ");
    db.setHostName(data.at(1).toLocal8Bit().constData());

    // Set Port Number
    line = config_file.readLine();
    data = line.split(" ");
    db.setPort(data.at(1).toInt());

    // Set Database Name
    line = config_file.readLine();
    data = line.split(" ");
    db.setDatabaseName(data.at(1).toLocal8Bit().constData());

    // Set User Name
    line = config_file.readLine();
    data = line.split(" ");
    db.setUserName(data.at(1).toLocal8Bit().constData());

    // Set Password
    line = config_file.readLine();
    data = line.split(" ");
    db.setPassword(data.at(1).toLocal8Bit().constData());

    // Close the file and open a connection
    file.close();

    if(!db.open()) {
        throw QString("Could not open a connection to the database!");
    }

    return db;
}
