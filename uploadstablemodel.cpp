#include "uploadstablemodel.h"

UploadsTableModel::UploadsTableModel(QObject *parent) :
    FileExchangeTableModel(parent),
    busy(false)
{

}

void UploadsTableModel::addNewUpload(QString address, QString filePath)
{
    beginInsertRows(QModelIndex(), 0, 0);

    UploadItem* newUpload = new UploadItem(this, address, filePath);
    connect(newUpload, SIGNAL(thisChanged(int,int,int)),this, SLOT(itemChanged(int,int,int)));
    connect(newUpload, SIGNAL(finished()),              this, SLOT(uploadFinished()));
    items.prepend(newUpload);
    itemsQueue.enqueue(newUpload);

    for (int i = 0; i < items.size(); ++i) {
        items[i]->setRow(i);
    }
    endInsertRows();

    checkQueue();
}

void UploadsTableModel::uploadFinished()
{
    busy = false;
    checkQueue();
}

void UploadsTableModel::checkQueue()
{
    if (!busy) {
        if (!itemsQueue.isEmpty()) {
            busy = true;
            emit itemsQueue.dequeue()->startUpload();
        }
    }
}

