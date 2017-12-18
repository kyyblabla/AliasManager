#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>
#include <QRegExp>

#define DEFAULT_USER_ALIAS_PATH ".useralias"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->homePath = QStandardPaths::standardLocations(QStandardPaths::StandardLocation::HomeLocation).at(0);
    this->init();

    this->on_tableWidget_itemSelectionChanged();

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
    disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    ui->tableWidget->setRowCount(this->aliasItemList.size());
    for (int i=0;i<this->aliasItemList.size();++i) {
        AliasItem * const aliasItem = this->aliasItemList.at(i);
        QTableWidgetItem *item1=new QTableWidgetItem();
        item1->setCheckState(Qt::CheckState::Checked);
        ui->tableWidget->setItem(i,0,item1);
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(aliasItem->alias));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(aliasItem->command));
    }

    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

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

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if(item->text().trimmed().isEmpty()){
        return;
    }
    if(item->row()>=this->aliasItemList.size()){
        this->aliasItemList.append(new AliasItem());
    }
    AliasItem * const aliasItem = this->aliasItemList.at(item->row());
    if(item->column()==1){
        aliasItem->alias=item->text().trimmed();
    }else if(item->column()==2){
        aliasItem->command=item->text().trimmed();
    }
    if(aliasItem->alias.isEmpty()||aliasItem->command.isEmpty()){
        return;
    }
    saveUserAlias();
}

void MainWindow::on_add_clicked()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);

    QTableWidgetItem *item=new QTableWidgetItem();
    item->setCheckState(Qt::CheckState::Checked);
    int row=ui->tableWidget->rowCount()-1;
    ui->tableWidget->setItem(row,0,item);
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(""));

    ui->tableWidget->selectRow(row);
    ui->tableWidget->selectColumn(1);

}

void MainWindow::on_remove_clicked()
{
    int localCurrentRow = ui->tableWidget->currentRow();
    this->aliasItemList.removeAt(localCurrentRow);
    ui->tableWidget->removeRow(localCurrentRow);
    saveUserAlias();
}

void MainWindow::on_tableWidget_itemSelectionChanged()
{
    ui->remove->setEnabled(ui->tableWidget->currentRow()!=-1);
}
