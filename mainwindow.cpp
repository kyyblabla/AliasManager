#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>
#include <QRegExp>
#include <QStandardItemModel>

#define DEFAULT_USER_ALIAS_PATH ".useralias"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->homePath = QStandardPaths::standardLocations(QStandardPaths::StandardLocation::HomeLocation).at(0);
    this->init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    this->initAliasItems();
    this->initAliasItemTable();
}

void MainWindow::initAliasItems()
{
    QString path=this->homePath+"/"+DEFAULT_USER_ALIAS_PATH;
    QFile file(path);

    if(!file.exists()){
        qDebug()<<"cannot find file:"<<path;
        return;
    }

    if (file.open(QFile::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if(line.startsWith("alias")){
                qDebug()<<line;
                QStringList defines=line.trimmed().replace(QRegExp("^#?alias\\s+"),"").split("=",QString::SplitBehavior::SkipEmptyParts);
                if(defines.isEmpty()){
                    continue;
                }
                bool use=!line.startsWith("#");
                QString alias=defines.at(0).trimmed();
                QString command=defines.at(1).trimmed().replace(QRegExp("^[\"']|[\"'']$"),"");
                this->aliasItemList.append(new AliasItem(alias,command,use));
            }
        }
        file.close();
    }else{
        qDebug()<<"cannot open file:"<<path;
    }
}



void MainWindow::initAliasItemTable()
{
    QStandardItemModel *model = new QStandardItemModel(3,3,this); //2 Rows and 3 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("alias")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("comand")));

    QStandardItem *firstRow = new QStandardItem(QString("ColumnValue"));
    model->setItem(1,1,firstRow);

    ui->tableView->setModel(model);
    ui->tableView->updateGeometry();
}

void MainWindow::saveUserAlias()
{
    QString path=this->homePath+"/"+DEFAULT_USER_ALIAS_PATH;
    QFile file(path);
    if (file.open(QFile::WriteOnly)) {
        QTextStream out(&file);
        foreach (AliasItem * const aliasItem, this->aliasItemList) {
            out<<"alias "<<aliasItem->alias<<"="<<"\""<<aliasItem->command<<"\""<<"\n";
        }
        out.flush();
        file.close();
    }

}


void MainWindow::on_add_clicked()
{
    //    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);

    //    QTableWidgetItem *item=new QTableWidgetItem();
    //    item->setCheckState(Qt::CheckState::Checked);
    //    int row=ui->tableWidget->rowCount()-1;
    //    ui->tableWidget->setItem(row,0,item);
    //    ui->tableWidget->setItem(row,1,new QTableWidgetItem(""));
    //    ui->tableWidget->setItem(row,2,new QTableWidgetItem(""));

    //    ui->tableWidget->selectRow(row);
    //    ui->tableWidget->selectColumn(1);

}

void MainWindow::on_remove_clicked()
{
    //    int localCurrentRow = ui->tableWidget->currentRow();
    //    this->aliasItemList.removeAt(localCurrentRow);
    //    ui->tableWidget->removeRow(localCurrentRow);
    //    saveUserAlias();
}

