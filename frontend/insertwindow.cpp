#include "mainwindow.h"
#include "insertwindow.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QtSql>

insertWindow::insertWindow() {

    // Create and set a grid layout
    QGridLayout *insertLayout = new QGridLayout;
    setLayout(insertLayout);
    resize(200, 500);

    // Instantiate the Labels
    l_title = new QLabel(tr("Title:"));
    l_title->show();
    insertLayout->addWidget(l_title, 0, 0);

    l_author = new QLabel(tr("Author:"));
    l_author->show();
    insertLayout->addWidget(l_author, 1, 0);

    l_magazine = new QLabel(tr("Magazine:"));
    l_magazine->show();
    insertLayout->addWidget(l_magazine, 2, 0);

    l_year = new QLabel(tr("Year:"));
    l_year->show();
    insertLayout->addWidget(l_year, 3, 0);

    l_pages = new QLabel(tr("Pages:"));
    l_pages->show();
    insertLayout->addWidget(l_pages, 4, 0);

    l_isbn = new QLabel(tr("ISBN:"));
    l_isbn->show();
    insertLayout->addWidget(l_isbn, 5, 0);

    l_keywords = new QLabel(tr("Keywords:"));
    l_keywords->show();
    insertLayout->addWidget(l_keywords, 6, 0);

    // Instantiate the Line Edits
    t_title = new QLineEdit();
    t_title->show();
    insertLayout->addWidget(t_title, 0, 1);

    t_author = new QLineEdit();
    t_author->show();
    insertLayout->addWidget(t_author, 1, 1);

    t_magazine = new QLineEdit();
    t_magazine->show();
    insertLayout->addWidget(t_magazine, 2, 1);

    t_year = new QLineEdit();
    t_year->show();
    insertLayout->addWidget(t_year, 3, 1);

    t_pages = new QLineEdit();
    t_pages->show();
    insertLayout->addWidget(t_pages, 4, 1);

    t_isbn = new QLineEdit();
    t_isbn->show();
    insertLayout->addWidget(t_isbn, 5, 1);

    t_keywords = new QLineEdit();
    t_keywords->show();
    insertLayout->addWidget(t_keywords, 6, 1);

    // Instantiate the Buttons
    submitButton = new QPushButton(tr("Submit"));
    submitButton->show();
    insertLayout->addWidget(submitButton, 7, 0);

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->show();
    insertLayout->addWidget(cancelButton, 7, 1);

    // Connect slots
    connect(submitButton, SIGNAL(clicked()), this, SLOT(sumbit()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

void insertWindow::sumbit() {
    QSqlDatabase db = MainWindow::connectDB();

    QSqlQuery query = db.exec("INSERT INTO users VALUES(4, 'DDD')");

    db.close();
}

// Emit a closedSignal
void insertWindow::cancel() {
    emit closedSignal();
}

// Emit a closedSignal
void insertWindow::closeEvent(QCloseEvent *event)
{
    emit closedSignal();
}
