#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    qDebug() << "Driver: " << QSqlDatabase::drivers();
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("../info.db");

    if (!mydb.open()) {
        QMessageBox::warning(this, "Error", mydb.lastError().text());
        return ;
    }

    query = QSqlQuery(mydb);
    //sqlite 语句以分号结尾
    //创建table时所使用的() 还是 {} 对于sqlite来说有影响
    //opeStr = "show tables;";
    //qDebug() << query.exec(opeStr);
    //while(query.next()) {
    //    qDebug() << query.value(0).toString();
    //}

    opeStr = "create table student (id int not null primary key, name varchar(255), score int(255));";

    query.exec(opeStr);


    opeStr = "insert into student (id, name, score) values (12, 'Mike', 90);";
    inserttoDb(query, opeStr);
/*
    opeStr = "select * from student;";
    selectFromDb(query, opeStr);

    opeStr = "delete from student;";
    query.exec(opeStr);

    opeStr = "select * from student;";
    selectFromDb(query, opeStr);
    */

    //指定父对象
    model = new QSqlTableModel(this);
    model->setTable("student");
    model->select();

    ui->tableView->setModel(model);

    //model->setHeaderData(0, Qt::Horizontal, "index");
    //更改model的提交方式
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //设置ui的edit模式
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

}

MyWidget::~MyWidget()
{
     mydb.close();
    delete ui;
}

void MyWidget::selectFromDb(QSqlQuery &query, const QString &str)
{
    bool flag = true;
    query.exec(str);

    while( (flag = query.next())) {
        qDebug() << query.value(0).toInt();
        qDebug() << query.value(1).toString();
        qDebug() << query.value(2).toInt();
    }

    if (!flag) {
        qDebug() << "The table is empty.";
        return ;
    }
}

bool MyWidget::inserttoDb(QSqlQuery &query, const QString &str)
{
    return query.exec(str);
}

void MyWidget::on_addButton_clicked()
{
    //获取空记录
    QSqlRecord record = model->record();
    //添加新纪录
    model->insertRecord(model->rowCount(), record);
}

void MyWidget::on_delButton_clicked()
{
    //获取选中的模型
    QItemSelectionModel * selModel =  ui->tableView->selectionModel();
    //获取选中模型中的index
    QModelIndexList list =  selModel->selectedRows();

    for (int i = 0; i < list.size(); i++) {
        qDebug() << list.at(i).row();
        //根据所在的index来删除对应的数据
        model->removeRow(list.at(i).row());
    }
    //model->select();
    selModel = NULL;
}

void MyWidget::on_confirmButton_clicked()
{
   //提交动作
   model->submitAll();

   model->setFilter("");
   model->select();
}

void MyWidget::on_cancelButton_clicked()
{
    //撤销动作
    model->revertAll();
    //提交动作
    model->submitAll();
}

void MyWidget::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    if (!name.isEmpty()) {
        QString str = QString("name = '%1'").arg(name);
        model->setFilter(str);
    } else {
        //清空过滤设置，并查询
        model->setFilter("");
        model->select();
    }
}
