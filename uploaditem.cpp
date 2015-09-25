#include "uploaditem.h"
#include "fileoperationsockets.h"

#include <QFileInfo>

UploadItem::UploadItem(QObject *parent, QString _address, QString filePath) :
    FileExchangeItem(parent)
{
    address = _address;
    QFileInfo fileInfo(filePath);
    fileName = fileInfo.fileName();
    fileSize = QString::number((double)fileInfo.size()/(1024.*1024.))+ "MB";
    status = "Queued";

    FileOperation::ClientSocket *fileClient = new FileOperation::ClientSocket(address, filePath);
    workerThread = new QThread(this);

    fileClient->moveToThread(workerThread);
    connect(workerThread,   SIGNAL(finished()),                         fileClient,     SLOT(deleteLater()));
    connect(this,           SIGNAL(startUpload()),                      fileClient,     SLOT(initTransfer()));
    connect(fileClient,     SIGNAL(disconnected()),                     workerThread,   SLOT(quit()));
    connect(fileClient,     SIGNAL(disconnected()),                     this,           SIGNAL(finished()));
    connect(fileClient,     SIGNAL(signalInfo(QString,QString,QString)),this,           SLOT(infoChanged(QString,QString,QString)));
    connect(fileClient,     SIGNAL(signalProgress(int)),                this,           SLOT(progressChanged(int)));
    connect(fileClient,     SIGNAL(signalStatus(QString)),              this,           SLOT(statusChanged(QString)));
    connect(fileClient,     SIGNAL(signalSpeed(QString)),               this,           SLOT(speedChanged(QString)));
    workerThread->start();
}

UploadItem::~UploadItem()
{
    workerThread->quit();
    workerThread->wait();
}
