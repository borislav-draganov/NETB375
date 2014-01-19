#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QApplication>
#include <QRadioButton>
#include <QPushButton>
#include <QFileDialog>
#include <QTableView>
#include <QLineEdit>
#include <QtCore>
#include <QFile>
#include <QtGui>
#include <QtSql>

class searchWindow : public QWidget
{
    Q_OBJECT

public:
    searchWindow(QWidget *parent = 0);
    QPushButton *goButton;          // Search query button for the Search window
    QPushButton *exportButton;      // Export button for exporting a selected row
    QPushButton *exportAllButton;   // Export button for exporting the whole result
    QPushButton *deleteButton;      // Delete button
    QPushButton *s_cancelButton;    // Cancel button for the Search window

public slots:
    void go();                      // Initiating the search query
    void s_cancel();                // Cancel slot for the Search window
    void delete_item();             // Delete the selected item
    void export_item();             // Exports only the selected row
    void exportAll();               // Exports all results
    void checkAuthor();             // Checks the Author button when text typed into the line edit
    void checkTitle();              // Checks the Title button when text typed into the line edit
    void checkKeyword();            // Checks the Keyword button when text typed into the line edit

signals:
    void closedSignal();            // A signal that is emitted when the window is closed

protected:
    void closeEvent(QCloseEvent *event);    // An event which is called when the window is closed
    void keyPressEvent(QKeyEvent* event);   // Check if keys are pressed - in this case it'll be the "Enter" keys

private:
    QRadioButton *author;
    QRadioButton *title;
    QRadioButton *keyword;

    QLineEdit *sl_author;
    QLineEdit *sl_title;
    QLineEdit *sl_keyword;

    QSqlQueryModel *model;          // A model where the output of the database will be shown
    QTableView *output;             // A view where the output will be shown

    QString bound_key;              // Store the key of the bound value of the last prepared statement (search)
    QString bound_value;            // Store the value of the bound value of the last prepared statement (search)
};

#endif // SEARCHWINDOW_H
