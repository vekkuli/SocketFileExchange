#include "fileexchangetablemodel.h"
#include "fileoperationsockets.h"

namespace FileExchange
{

BaseTableModel::BaseTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int BaseTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return items.size();
}

int BaseTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant BaseTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.row() < items.size()) {
        return items.at(index.row())->data(index.column());
    }
    return QVariant();
}

QVariant BaseTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0: return QString("File");
            case 1: return QString("Progress");
            case 2: return QString("Size");
            case 3: return QString("Speed");
            case 4: return QString("Status");
            case 5: return QString("Address");
            }
        }
    }
    return QVariant();
}

void BaseTableModel::itemChanged(int row, int col1, int col2)
{
    emit dataChanged(createIndex(row, col1), createIndex(row, col2));
}

int BaseTableModel::addItem(FileExchangeItem *newItem)
{
    beginInsertRows(QModelIndex(), items.size(), items.size());

    connect(newItem, SIGNAL(thisChanged(int,int,int)),this, SLOT(itemChanged(int,int,int)));
    connect(newItem, SIGNAL(finished(int)),           this, SLOT(itemFinished(int)));
    items.append(newItem);

    for (int i = 0; i < items.size(); ++i)
        items[i]->setRow(i);

    endInsertRows();

    return newItem->getRow();
}

UploadsTableModel::UploadsTableModel(QObject *parent) :
    BaseTableModel(parent),
    busy(false)
{
}

void UploadsTableModel::addNewUpload(QString address, QString filePath)
{
    FileOperation::ClientSocket* newSocket = new FileOperation::ClientSocket(address, filePath);
    FileExchangeItem* newUpload = new FileExchangeItem(this, newSocket, "Queued", address, filePath);
    addItem(newUpload);
    itemsQueue.enqueue(newUpload);
    checkQueue();
}

void UploadsTableModel::itemFinished(int row)
{
    Q_UNUSED(row);
    busy = false;
    checkQueue();
}

void UploadsTableModel::checkQueue()
{
    if (!busy) {
        if (!itemsQueue.isEmpty()) {
            busy = true;
            emit itemsQueue.dequeue()->startOperation();
        }
    }
}

DownloadsTableModel::DownloadsTableModel(QObject *parent) :
    BaseTableModel(parent)
{
}

void DownloadsTableModel::addNewDownload(qintptr socketDescriptor, QString downloadFolder)
{
    FileOperation::ServerSocket* newSocket = new FileOperation::ServerSocket(socketDescriptor, downloadFolder);
    FileExchangeItem* newDownload = new FileExchangeItem(this, newSocket, "Waiting", QString(), QString());
    addItem(newDownload);
    emit newDownload->startOperation();
}

void DownloadsTableModel::itemFinished(int row)
{
    Q_UNUSED(row);
}

} // namespace

