#include "fileexchangetablemodel.h"

FileExchangeTableModel::FileExchangeTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

int FileExchangeTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return items.size();
}

int FileExchangeTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant FileExchangeTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.row() < items.size()) {
        return items.at(index.row())->data(index.column());
    }
    return QVariant();
}

QVariant FileExchangeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("File");
            case 1:
                return QString("Progress");
            case 2:
                return QString("Size");
            case 3:
                return QString("Speed");
            case 4:
                return QString("Status");
            case 5:
                return QString("Address");
            }
        }
    }
    return QVariant();
}

void FileExchangeTableModel::itemChanged(int row, int col1, int col2)
{
    emit dataChanged(createIndex(row, col1), createIndex(row, col2));
}

