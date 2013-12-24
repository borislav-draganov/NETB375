#include "searchwindow.h"
#include "mainwindow.h"

#include <QGridLayout>
#include <QMessageBox>

searchWindow::searchWindow()
{
    // Create and set Grid Layout
    QGridLayout *searchLayout = new QGridLayout(this);
    setLayout(searchLayout);
    resize(800, 600);

    // Instantiate the Radio Buttons
    author = new QRadioButton(this);
    author->setText("Search by author");
    author->show();
    searchLayout->addWidget(author, 0, 0);

    title = new QRadioButton(this);
    title->setText("Search by title");
    title->show();
    searchLayout->addWidget(title, 1, 0);

    keyword = new QRadioButton(this);
    keyword->setText("Search by keyword");
    keyword->show();
    searchLayout->addWidget(keyword, 2, 0);

    // Instantiate the Line Edits
    sl_author = new QLineEdit(this);
    sl_author->show();
    searchLayout->addWidget(sl_author, 0, 1);

    sl_title = new QLineEdit(this);
    sl_title->show();
    searchLayout->addWidget(sl_title, 1, 1);

    sl_keyword = new QLineEdit(this);
    sl_keyword->show();
    searchLayout->addWidget(sl_keyword, 2, 1);

    // Instantiate the view
    model = new QSqlQueryModel(this);
    output = new QTableView(this);
    output->setModel(model);
    searchLayout->addWidget(output, 3, 0, 3, 4);

    // Instantiate the Push Buttons
    goButton = new QPushButton(this);
    goButton->setText("GO!");
    goButton->show();
    searchLayout->addWidget(goButton, 6, 0);

    exportButton = new QPushButton(this);
    exportButton->setText("Export");
    exportButton->show();
    searchLayout->addWidget(exportButton, 6, 1, Qt::AlignRight);

    deleteButton = new QPushButton(this);
    deleteButton->setText("Delete");
    deleteButton->show();
    searchLayout->addWidget(deleteButton, 6, 2);

    s_cancelButton = new QPushButton(this);
    s_cancelButton->setText("Cancel");
    s_cancelButton->show();
    searchLayout->addWidget(s_cancelButton, 6, 3);

    // Connect Slots
    connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(delete_item()));
    connect(s_cancelButton, SIGNAL(clicked()), this, SLOT(s_cancel()));
}

void searchWindow::go()
{
    try
    {
        // Connect to the base and make a query
        QSqlDatabase db = MainWindow::connectDB();
        QSqlQuery query(db);

        if (author->isChecked())
        {
            if (sl_author->text() == "") { throw QString("Type author name to search"); }
            else
            {
                // Prepare the query
                query.prepare("SELECT * from searchByAuthor(:author);");

                // Bind the value
                query.bindValue(":author", "%" + sl_author->text() + "%");
                query.exec();
                model->setQuery(query);
            }
        }
        else if (title->isChecked())
        {
            if (sl_title->text() == "") { throw QString("Type title name to search"); }
            else
            {
                // Prepare the query
                query.prepare("SELECT * from searchByTitle(:title);");

                // Bind the value
                query.bindValue(":title", "%" + sl_title->text() + "%");
                query.exec();
                model->setQuery(query);
            }
        }
        else if (keyword->isChecked())
        {
            if (sl_keyword->text() == "") { throw QString("Type a keyword to search"); }
            else
            {
                // Prepare the query
                query.prepare("SELECT * from searchByKeyword(:keyword);");

                // Bind the value
                query.bindValue(":keyword", "%" + sl_keyword->text() + "%");
                query.exec();
                model->setQuery(query);
            }
        }
        else { model->setQuery("Select * FROM \"Books\""); }

        db.close();
    }
    catch(QString Err)
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), Err);
    }
}

void searchWindow::delete_item()
{
    try
    {
        // Get the index of the currently selected row
        int selected_row = output->selectionModel()->currentIndex().row();

        // if it's -1 then no row is selected
        if (selected_row < 0) {
            throw QString("No row selected!");
        }

        // Get the isbn of the selected row
        QString isbn = model->data(model->index(selected_row,5)).toString();

        // Attempt to connect to the database and make a query
        QSqlDatabase db = MainWindow::connectDB();
        QSqlQuery query(db);

        // Prepare the Query
        query.prepare("SELECT * from dellBook(:isbn);");

        // Bind the value
        query.bindValue(":isbn", isbn);

        // Execute the query and close the database connection
        query.exec();

        // Resend the last queyr used to make the model (repeat the search)
        QString last_search = model->query().lastQuery();
        model->setQuery(last_search);

        // Close the connection
        db.close();
    }
    catch(QString Err)
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), Err);
    }
}

void searchWindow::s_cancel()
{
    emit closedSignal();
}

// Emit a closedSignal
void searchWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    emit closedSignal();
}
