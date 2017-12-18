#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class QTableWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    class AliasItem{
    public:
        AliasItem(){}
        AliasItem(QString alias, QString command,bool use=true){
            this->alias=alias;
            this->command=command;
            this->use=use;
        }
        bool use;
        QString alias;
        QString command;
    };

private slots:
    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_add_clicked();
    void on_remove_clicked();

    void on_tableWidget_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
    QList<AliasItem*> aliasItemList;
    QString homePath;
    void init();
    void initAliasItems();
    void initAliasItemTable();
    void saveUserAlias();
};

#endif // MAINWINDOW_H
