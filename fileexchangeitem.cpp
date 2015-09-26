#include "fileexchangeitem.h"
#include <QFileInfo>

FileExchangeItem::FileExchangeItem(QObject *parent, FileOperation::Socket* socket, QString _status, QString _address, QString _filePath) :
    QObject(parent),
    workerThread(new QThread(this)),
    socket(socket),
    address(_address),
    progress(0),
    status(_status)
{
    if (!_filePath.isEmpty()) {
        QFileInfo fileInfo(_filePath);
        fileName = fileInfo.fileName();
        fileSize = QString::number((double)fileInfo.size()/(1048576.))+ "MB";
    }

    socket->moveToThread(workerThread);
    connect(workerThread,   SIGNAL(finished()),                         socket,         SLOT(deleteLater()));   
    connect(this,           SIGNAL(startOperation()),                   socket,         SLOT(onStartOperation()));
    connect(this,           SIGNAL(finished(int)),                      workerThread,   SLOT(quit()));
    connect(socket,         SIGNAL(disconnected()),                     this,           SLOT(onSocketDisconnected()));
    connect(socket,         SIGNAL(signalAddress(QString)),             this,           SLOT(addressChanged(QString)));
    connect(socket,         SIGNAL(signalFileName(QString)),            this,           SLOT(fileNameChanged(QString)));
    connect(socket,         SIGNAL(signalFileSize(QString)),            this,           SLOT(fileSizeChanged(QString)));
    connect(socket,         SIGNAL(signalProgress(int)),                this,           SLOT(progressChanged(int)));
    connect(socket,         SIGNAL(signalStatus(QString)),              this,           SLOT(statusChanged(QString)));
    connect(socket,         SIGNAL(signalSpeed(QString)),               this,           SLOT(speedChanged(QString)));
    workerThread->start();
}

FileExchangeItem::~FileExchangeItem()
{
    workerThread->quit();
    workerThread->wait();
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

void FileExchangeItem::addressChanged(QString value)
{
    address = value;
    emit thisChanged(row, 5, 5);
}

void FileExchangeItem::fileNameChanged(QString value)
{
    fileName = value;
    emit thisChanged(row, 0, 0);
}

void FileExchangeItem::fileSizeChanged(QString value)
{
    fileSize = value;
    emit thisChanged(row, 2, 2);
}

void FileExchangeItem::progressChanged(int value)
{
    progress = value;
    emit thisChanged(row, 1, 1);
}

void FileExchangeItem::statusChanged(QString value)
{
    status = value;
    emit thisChanged(row, 4, 4);
}

void FileExchangeItem::speedChanged(QString value)
{
    speed = value;
    emit thisChanged(row, 3, 3);
}

void FileExchangeItem::onSocketDisconnected()
{
    emit finished(row);
}

int FileExchangeItem::getRow() const
{
    return row;
}

void FileExchangeItem::setRow(int value)
{
    row = value;
}
