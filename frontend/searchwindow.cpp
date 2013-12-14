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

    s_cancelButton = new QPushButton(this);
    s_cancelButton->setText("Cancel");
    s_cancelButton->show();
    searchLayout->addWidget(s_cancelButton, 6, 2);

    // Connect Slots
    connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
    connect(s_cancelButton, SIGNAL(clicked()), this, SLOT(s_cancel()));
}

void searchWindow::go()
{
    try
    {
        QSqlDatabase db = MainWindow::connectDB();

        model->setQuery("SELECT * FROM \"Books\";");

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
    emit closedSignal();
}
