#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QItemSelectionModel>
namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

    void selectFromDb(QSqlQuery &query, const QString &str);
    bool inserttoDb(QSqlQuery &query, const QString &str);

private slots:
    void on_addButton_clicked();

    void on_delButton_clicked();

    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::MyWidget *ui;
    QSqlDatabase mydb;
    QSqlQuery query;
    QString opeStr;
    QSqlTableModel *model;
};

#endif // MYWIDGET_H
