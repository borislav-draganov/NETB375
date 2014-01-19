#include "mainwindow.h"
#include "insertwindow.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QtSql>

// Constructor
insertWindow::insertWindow(QWidget *parent) :
    QWidget(parent){

    // Create and set a Grid Layout
    QGridLayout *insertLayout = new QGridLayout(this);
    setLayout(insertLayout);
    resize(200, 500);
    setWindowFlags(Qt::Window);

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
    l_isbn->setText(tr("ISBN/ISSN:"));
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

// Performs a validation of the user-input and if it's correct - insert the data into the database
void insertWindow::submit() {
    try {
        // Check if all fields are filled in
        if (t_title->text() == "" ||
                t_author->text() == "" ||
                t_magazine->text() == "" ||
                t_year->text() == "" ||
                t_pages->text() == "" ||
                t_isbn->text() == "" ||
                t_keywords->text() == "")
        {
            throw QString("Not all fields have been filled in!");
        }

        // Check if the input value for Year is a four digit number
        QRegExp year_check("^\\d{4}$");
        if (year_check.indexIn(t_year->text()) == -1) {
            throw QString("Incorrect format for year! (YYYY)");
        }

        // Check if the input value for Pages is a number
        QRegExp pages_check("^\\d+$");
        if (pages_check.indexIn(t_pages->text()) == -1) {
            throw QString("Pages must be a number!");
        }

        // Remove spaces and dashes from the ISBN/ISSN
        QString isbn = t_isbn->text();
        isbn.replace(" ", "");
        isbn.replace("-", "");

        // Check if the ISBN/ISSN is valid
        if (!is_valid_isbn(isbn)) {
            throw QString("Incorrect ISBN/ISSN!");
        }

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
        query.bindValue(":isbn", isbn);
        query.bindValue(":keywords", "{" + t_keywords->text() + "}");

        // Execute the query and close the database connection
        query.exec();
        db.close();

        // Check the result
        query.next();

		// Notify the user of the result
        if(query.value(0).toInt() == 0) {
            QMessageBox::critical(this, QObject::tr("Error"), "The supplied ISSN/ISBN already exists in the database! Did not insert!");
        }
        else if(query.value(0).toInt() == 1) {
            QMessageBox::information(this, QObject::tr("Success"), "Insert successful!");

            // Close the window
            emit closedSignal();
        }
    }
    // Display the error if there is such
    catch(QString Err) {
        QMessageBox::critical(this, QObject::tr("Error"), Err);
    }
}

// Emit a closedSignal - when the Close button is pressed
void insertWindow::cancel() {
    emit closedSignal();
}

// Emit a closedSignal - when the window is closed from the corner button
void insertWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    emit closedSignal();
}

// If an "Enter" key is pressed, do the insertion
void insertWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        submit();
    }
}

/* Check if the given string of digits is a valid ISSN, ISBN-10 or ISBN-13
 * @param: string of digits only
 * @return: true if it's correct, false if not
 */
bool insertWindow::is_valid_isbn(QString str) {
	// In case the user inputs an ISSN or ISBN-10 with a small "x"
    str.toUpper();
	
    // Make sure there are only digits or ends with an X (in case it's a ISSN or ISBN-10)
    QRegExp all_digit("^\\d+$");
    QRegExp issn("^\\d+X$");
    if (all_digit.indexIn(str) == -1 && issn.indexIn(str) == -1) {
        return false;
    }

    // Check for a valid ISSN
    if (str.length() == 8) {
        int length = str.length();
        int check_digit = 0;

        // Sum the digits
        for (int i = 0; i < length - 1; i++) {
            check_digit += str.mid(i, 1).toInt() * (length-i);
        }

        // Modulus 11
        check_digit = check_digit % 11;

        // Remainder subtracted from 11
        check_digit = 11 - check_digit;

        // If the last character is X, the check digit has to be 10
        if (str.mid(length-1, 1) == "X") {
            return check_digit == 10;
        }

        // Safely convert the last character into an integer
        int last_digit = str.mid(length-1, 1).toInt();

        // If the checksum before the subtraction from 11 was 0, the final digit has to be 0
        if (check_digit == 11) {
            return last_digit == 0;
        }
        // In all other cases, the last digit and the check_digit have to match
        return check_digit == last_digit;
    }
    // Check for a valid ISBN-10
    else if (str.length() == 10) {
        int length = str.length();
        int check_digit = 0;

        // Sum the digits
        for (int i = 0; i < length - 1; i++) {
            check_digit += str.mid(i, 1).toInt() * (length-i);
        }

        if (str.mid(length-1, 1) == "X") {
            check_digit += 10;
        }
        else {
            check_digit += str.mid(length-1, 1).toInt();
        }

        // Modulus 11
        return check_digit % 11 == 0;
    }
    // Check for a valid ISBN-13
    else if (str.length() == 13) {
        int check_digit = 0;

        for (int i = 0; i < 13; i += 2) {
            check_digit += str.mid(i, 1).toInt();
        }

        for (int i = 1; i < 12; i += 2) {
            check_digit += str.mid(i, 1).toInt() * 3;
        }

        return check_digit % 10 == 0;
    }

    // Anything else is incorrect
    return false;
}
