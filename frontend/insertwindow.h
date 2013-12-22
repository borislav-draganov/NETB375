#ifndef INSERTWINDOW_H
#define INSERTWINDOW_H

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QtGui>

class insertWindow : public QWidget
{
    Q_OBJECT

public:
    insertWindow();
    QPushButton *submitButton;
    QPushButton *cancelButton;

public slots:
    void submit();
    void cancel();

signals:
    void closedSignal();                    // A singal that is emited when the window is closed

protected:
    void closeEvent(QCloseEvent *event);    // An event which is called when the window is closed

private:
    QLabel *l_title;
    QLabel *l_author;
    QLabel *l_magazine;
    QLabel *l_year;
    QLabel *l_pages;
    QLabel *l_isbn;
    QLabel *l_keywords;

    QLineEdit *t_title;
    QLineEdit *t_author;
    QLineEdit *t_magazine;
    QLineEdit *t_year;
    QLineEdit *t_pages;
    QLineEdit *t_isbn;
    QLineEdit *t_keywords;

    bool is_valid_isbn(QString str); // A function that will check if the sublied string is a correct ISBN or ISSN
};

#endif // INSERTWINDOW_H
