#include "searchwindow.h"
#include "mainwindow.h"

#include <QGridLayout>
#include <QMessageBox>

// Constructor
searchWindow::searchWindow(QWidget *parent) : QWidget(parent)
{
    // Create and set Grid Layout
    QGridLayout *searchLayout = new QGridLayout(this);
    setLayout(searchLayout);
    resize(800, 600);
    setWindowFlags(Qt::Window);

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
    searchLayout->addWidget(output, 3, 0, 3, 5);

    // Instantiate the Push Buttons
    goButton = new QPushButton(this);
    goButton->setText("GO!");
    goButton->show();
    searchLayout->addWidget(goButton, 6, 0);

    exportButton = new QPushButton(this);
    exportButton->setText("Export");
    exportButton->show();
    searchLayout->addWidget(exportButton, 6, 1, Qt::AlignRight);

    exportAllButton = new QPushButton(this);
    exportAllButton->setText("Export All");
    exportAllButton->show();
    searchLayout->addWidget(exportAllButton, 6, 2);

    deleteButton = new QPushButton(this);
    deleteButton->setText("Delete");
    deleteButton->show();
    searchLayout->addWidget(deleteButton, 6, 3);

    s_cancelButton = new QPushButton(this);
    s_cancelButton->setText("Cancel");
    s_cancelButton->show();
    searchLayout->addWidget(s_cancelButton, 6, 4);

    // Connect Slots
    connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(delete_item()));
    connect(s_cancelButton, SIGNAL(clicked()), this, SLOT(s_cancel()));
    connect(exportButton, SIGNAL(clicked()), this, SLOT(export_item()));
    connect(exportAllButton, SIGNAL(clicked()), this, SLOT(exportAll()));
    connect(sl_author, SIGNAL(textEdited(QString)),this, SLOT(checkAuthor()));
    connect(sl_title, SIGNAL(textEdited(QString)),this, SLOT(checkTitle()));
    connect(sl_keyword, SIGNAL(textEdited(QString)),this, SLOT(checkKeyword()));
}

// Checks the Author button when text typed into the line edit
void searchWindow::checkAuthor()
{
    author->click();
}

// Checks the Title button when text typed into the line edit
void searchWindow::checkTitle()
{
    title->click();
}

// Checks the Keyword button when text typed into the line edit
void searchWindow::checkKeyword()
{
    keyword->click();
}

// Performs the search by the given criteria, if there is a match, it is displayed in the window
void searchWindow::go()
{
    try
    {
        // Clear the model, before the new search
        model->clear();

        // Connect to the base and make a query
        QSqlDatabase db = MainWindow::connectDB();
        QSqlQuery query(db);

        // Checks if the button for searching by author is checked, if it is, the searching is made by author
        if (author->isChecked())
        {
            if (sl_author->text() == "") { model->setQuery("Select * FROM \"Books\""); }
            else
            {
                // Prepare the query
                query.prepare("SELECT * from searchByAuthor(:author);");

                // Save the bound key and value
                bound_key = ":author";
                bound_value = "%" + sl_author->text() + "%";

                // Bind the value
                query.bindValue(bound_key, bound_value);
                query.exec();
                model->setQuery(query);
            }
        }
        // Checks if the button for searching by title is checked, if it is, the searching is made by title
        else if (title->isChecked())
        {
            if (sl_title->text() == "") { model->setQuery("Select * FROM \"Books\""); }
            else
            {
                // Prepare the query
                query.prepare("SELECT * from searchByTitle(:title);");

                // Save the bound key and value
                bound_key = ":title";
                bound_value = "%" + sl_title->text() + "%";

                // Bind the value
                query.bindValue(bound_key, bound_value);
                query.exec();
                model->setQuery(query);
            }
        }
        // Checks if the keyword for searching by author is checked, if it is, the searching is made by keyword
        else if (keyword->isChecked())
        {
            if (sl_keyword->text() == "") { model->setQuery("Select * FROM \"Books\""); }
            else
            {
                // Prepare the query
                query.prepare("SELECT * from searchByKeyword(:keyword);");

                // Save the bound key and value
                bound_key = ":keyword";
                bound_value = "%" + sl_keyword->text() + "%";

                // Bind the value
                query.bindValue(bound_key, bound_value);
                query.exec();
                model->setQuery(query);
            }
        }
        else { model->setQuery("Select * FROM \"Books\""); }

        db.close();
    }
    // Display the error if there is such
    catch(QString Err)
    {
        QMessageBox::critical(this, QObject::tr("Error"), Err);
    }
}

// Exports a single row as a BiBTeX entry
void searchWindow::export_item()
{
    try
    {
        // Create and save the file
        QString filename = QFileDialog::getSaveFileName(this, "Save File", "", "BiBTeX (*.bib)");

        // Check if filename is empty (i.e. Cancel is pressed)
        if (filename == "") { return; }

        QFile exportFile(filename);
        if (!exportFile.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::information(this, "Error opening file", "Unable to open the file");
            return;
        }

        // Get the index of the currently selected row
        int selected_row = output->selectionModel()->currentIndex().row();

        // if it's -1 then no row is selected
        if (selected_row < 0)
        {
            throw QString("No row selected!");
        }

        // Collect the data and prepare it for exporting
        QString title = model->data(model->index(selected_row, 0)).toString();
        QString author = model->data(model->index(selected_row, 1)).toString();
        QString magazine = model->data(model->index(selected_row, 2)).toString();
        QString year = model->data(model->index(selected_row, 3)).toString();
        QString pages = model->data(model->index(selected_row, 4)).toString();
        QString isbn = model->data(model->index(selected_row, 5)).toString();
        QString keywords = model->data(model->index(selected_row, 6)).toString();

        QString input = "@misc{ exporting, title = " + title + ","
                + " author = " + author + ","
                + " magazine = " + magazine + ","
                + " year = " + year + ","
                + " pages = " + pages + ","
                + " isbn = " + isbn + ","
                + " keywords = " + keywords + "}";

        // Write the data into the file
        QTextStream out(&exportFile);
        out << input;

        // Flushing and closing
        exportFile.flush();
        exportFile.close();
    }
    // Display the error if there is such
    catch(QString Err)
    {
        QMessageBox::critical(this, QObject::tr("Error"), Err);
    }
}

// Exports all rows as a BiBTeX entry
void searchWindow::exportAll()
{
    try
    {
        // Create and save the file
        QString filename = QFileDialog::getSaveFileName(this, "Save File", "", "BiBTeX (*.bib)");

        // Check if filename is empty (i.e. Cancel is pressed)
        if (filename == "") { return; }

        QFile exportFile(filename);
        if (!exportFile.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::information(this, "Error opening file", "Unable to open the file");
            return;
        }

        // Get the count of the rows
        int row_count = model->rowCount();

        // Collect the data and prepare it for exporting
        for (int row = 0; row < row_count; row++)
        {
            QString title = model->data(model->index(row, 0)).toString();
            QString author = model->data(model->index(row, 1)).toString();
            QString magazine = model->data(model->index(row, 2)).toString();
            QString year = model->data(model->index(row, 3)).toString();
            QString pages = model->data(model->index(row, 4)).toString();
            QString isbn = model->data(model->index(row, 5)).toString();
            QString keywords = model->data(model->index(row, 6)).toString();

            QString input = "@misc{ exporting, title = " + title + ","
                    + " author = " + author + ","
                    + " magazine = " + magazine + ","
                    + " year = " + year + ","
                    + " pages = " + pages + ","
                    + " isbn = " + isbn + ","
                    + " keywords = " + keywords + "}" + '\n';

            // Write the data into the file
            QTextStream out(&exportFile);
            out << input;
        }

        // Flushing and closing
        exportFile.flush();
        exportFile.close();
    }
    // Display the error if there is such
    catch(QString Err)
    {
        QMessageBox::critical(this, QObject::tr("Error"), Err);
    }
}

// Deletes the selected row from the database
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

        // Save the last query used to make the model
        QString last_search = model->query().lastQuery();

        // Clear the model, before the new search
        model->clear();

        // Attempt to connect to the database and make a query
        QSqlDatabase db = MainWindow::connectDB();
        QSqlQuery query(db);

        // Prepare the Query
        query.prepare("SELECT * from dellBook(:isbn);");

        // Bind the value
        query.bindValue(":isbn", isbn);

        // Execute the query to delete the item
        query.exec();

        // Check the result
        query.next();

        if(query.value(0).toInt() == 0) {
            QMessageBox::critical(this, QObject::tr("Error"), "The item was not deleted!");
        }
        else if(query.value(0).toInt() == 1) {
            QMessageBox::information(this, QObject::tr("Success"), "Delete successful!");
        }

        // Resend the last query (repeat the search)
        query.prepare(last_search);
        query.bindValue(bound_key, bound_value);
        query.exec();

        model->setQuery(query);

        // Close the connection
        db.close();
    }
    // Display the error if there is such
    catch(QString Err)
    {
        QMessageBox::critical(this, QObject::tr("Error"), Err);
    }
}

// Close the window when the "Cancel" button is pressed
void searchWindow::s_cancel()
{
    emit closedSignal();
}

// Emit a closedSignal - when the window is closed from the corner button
void searchWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    emit closedSignal();
}

// If an "Enter" key is pressed, do the search
void searchWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        go();
    }
}
