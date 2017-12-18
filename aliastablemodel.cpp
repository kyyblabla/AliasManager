#include "aliastablemodel.h"

#include <QVariant>
#include <QDebug>

static const QString HEADERS[3]={"","alias","command"};

static QString  DATAS[4][3]={
    {"a","aaa","bbb"},
    {"c","ccc","ddd"},
    {"e","eee","fff"},
    {"g","ggg","hhh"}
};

AliasTableModel::AliasTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant AliasTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal){
        return HEADERS[section];
    }

    return QVariant();

}

bool AliasTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


int AliasTableModel::rowCount(const QModelIndex &parent) const
{
//    if (!parent.isValid()){
//        qDebug()<<"yyy";
//        return 0;
//    }
    return 4;
}

int AliasTableModel::columnCount(const QModelIndex &parent) const
{
//    if (!parent.isValid()){
//        qDebug()<<"xxx";
//        return 0;
//    }
    return 3;
}

QVariant AliasTableModel::data(const QModelIndex &index, int role) const
{
//    if (!index.isValid()){
//        return QVariant();
//    }

    return QVariant(DATAS[index.row()][index.column()]);
}

bool AliasTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if (data(index, role) != value) {
        // FIXME: Implement me!
        DATAS[index.row()][index.column()]=value.toString();
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags AliasTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool AliasTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}

bool AliasTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool AliasTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}

bool AliasTableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}
