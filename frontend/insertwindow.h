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
    void sumbit();
    void cancel();

signals:
    void closedSignal();                    // A singal that is emited when the windows is closed

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
};

#endif // INSERTWINDOW_H
