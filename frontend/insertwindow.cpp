#include "mainwindow.h"
#include "insertwindow.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QtSql>

insertWindow::insertWindow() {

    // Create and set a Grid Layout
    QGridLayout *insertLayout = new QGridLayout(this);
    setLayout(insertLayout);
    resize(200, 500);

    // Instantiate the Labels
    l_title = new QLabel(this);
    l_title->setText(tr("Title:"));
    l_title->show();
    insertLayout->addWidget(l_title, 0, 0);

    l_author = new QLabel(this);
    l_author->setText(tr("Author:"));
    l_author->show();
    insertLayout->addWidget(l_author, 1, 0);

    l_magazine = new QLabel(this);
    l_magazine->setText(tr("Magazine:"));
    l_magazine->show();
    insertLayout->addWidget(l_magazine, 2, 0);

    l_year = new QLabel(this);
    l_year->setText(tr("Year:"));
    l_year->show();
    insertLayout->addWidget(l_year, 3, 0);

    l_pages = new QLabel(this);
    l_pages->setText(tr("Pages:"));
    l_pages->show();
    insertLayout->addWidget(l_pages, 4, 0);

    l_isbn = new QLabel(this);
    l_isbn->setText(tr("ISBN:"));
    l_isbn->show();
    insertLayout->addWidget(l_isbn, 5, 0);

    l_keywords = new QLabel(this);
    l_keywords->setText(tr("Keywords:"));
    l_keywords->show();
    insertLayout->addWidget(l_keywords, 6, 0);

    // Instantiate the Line Edits
    t_title = new QLineEdit(this);
    t_title->show();
    insertLayout->addWidget(t_title, 0, 1);

    t_author = new QLineEdit(this);
    t_author->show();
    insertLayout->addWidget(t_author, 1, 1);

    t_magazine = new QLineEdit(this);
    t_magazine->show();
    insertLayout->addWidget(t_magazine, 2, 1);

    t_year = new QLineEdit(this);
    t_year->show();
    insertLayout->addWidget(t_year, 3, 1);

    t_pages = new QLineEdit(this);
    t_pages->show();
    insertLayout->addWidget(t_pages, 4, 1);

    t_isbn = new QLineEdit(this);
    t_isbn->show();
    insertLayout->addWidget(t_isbn, 5, 1);

    t_keywords = new QLineEdit(this);
    t_keywords->show();
    insertLayout->addWidget(t_keywords, 6, 1);

    // Instantiate the Buttons
    submitButton = new QPushButton(this);
    submitButton->setText(tr("Submit"));
    submitButton->show();
    insertLayout->addWidget(submitButton, 7, 0);

    cancelButton = new QPushButton(this);
    cancelButton->setText(tr("Cancel"));
    cancelButton->show();
    insertLayout->addWidget(cancelButton, 7, 1);

    // Connect Slots
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

void insertWindow::submit() {
    try {
        // Attempt to connect to the database and make a query
        QSqlDatabase db = MainWindow::connectDB();
        QSqlQuery query(db);

        // Prepare the Query
        query.prepare("SELECT * from fillBook(:title, :author, :magazine, :year, :pages, :isbn, :keywords);");

        // Bind the values
        query.bindValue(":title", t_title->text());
        query.bindValue(":author", t_author->text());
        query.bindValue(":magazine", t_magazine->text());
        query.bindValue(":year", t_year->text());
        query.bindValue(":pages", t_pages->text());
        query.bindValue(":isbn", t_isbn->text());
        query.bindValue(":keywords", "{" + t_keywords->text() + "}");

        // Execute the query and close the database connection
        query.exec();
        db.close();

        // Close the window
        emit closedSignal();
    }
    catch(QString Err) {
        QMessageBox::critical(NULL, QObject::tr("Error"), Err);
    }
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
