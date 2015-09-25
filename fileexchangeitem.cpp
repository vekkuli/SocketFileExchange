#include "fileexchangeitem.h"
#include <QThread>

FileExchangeItem::FileExchangeItem(QObject *parent, QString address, QString filePath) :
    QObject(parent),
    address(address),
    fileName("?"),
    fileSize("?"),
    progress(0),
    speed("")
{

}

QVariant FileExchangeItem::data(int column)
{
    switch (column)
    {
    case 0: return fileName;
    case 1: return progress;
    case 2: return fileSize;
    case 3: return speed;
    case 4: return status;
    case 5: return address;
    default: return QVariant();
    }
}

void FileExchangeItem::infoChanged(QString _fileName, QString _address, QString _fileSize)
{
    address = _address;
    fileName = _fileName;
    fileSize = _fileSize;
    emit thisChanged(row, 0, 5);
}

void FileExchangeItem::progressChanged(int _progress)
{
    progress = _progress;
    emit thisChanged(row, 1, 1);
}

void FileExchangeItem::statusChanged(QString _status)
{
    status = _status;
    emit thisChanged(row, 4, 4);
}

void FileExchangeItem::speedChanged(QString _speed)
{
    speed = _speed;
    emit thisChanged(row, 3, 3);
}

void FileExchangeItem::setRow(int value)
{
    row = value;
}
