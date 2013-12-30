#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QApplication>
#include <QRadioButton>
#include <QPushButton>
#include <QTableView>
#include <QLineEdit>
#include <QtCore>
#include <QtGui>
#include <QtSql>

class searchWindow : public QWidget
{
    Q_OBJECT

public:
    searchWindow(QWidget *parent = 0);
    QPushButton *goButton;          // Search query button for the Search window
    QPushButton *exportButton;      // Export button
    QPushButton *deleteButton;      // Delete button
    QPushButton *s_cancelButton;    // Cancel button for the Search window

public slots:
    void go();                      // Initiating the search querry
    void s_cancel();                // Cancel slot for the Search window
    void delete_item();             // Delete the selected item

signals:
    void closedSignal();

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

    QString bound_key;              // Store the key of the bound value of the last preprared statament (search)
    QString bound_value;            // Store the value of the bound value of the last preprared statament (search)
};

#endif // SEARCHWINDOW_H
