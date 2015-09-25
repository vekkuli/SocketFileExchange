#include "fileserver.h"
#include "threadedfileserver.h"
#include <QThread>

FileServer::FileServer(QObject *parent)
    : QTcpServer(parent)
{
}

void FileServer::incomingConnection(qintptr socketDescriptor)
{
    ThreadedFileServer *threadedFileServer = new ThreadedFileServer(socketDescriptor, downloadFolder);
    QThread *workerThread = new QThread(this);

    threadedFileServer->moveToThread(workerThread);
    connect(workerThread,               SIGNAL(finished()),     threadedFileServer, SLOT(deleteLater()));
    connect(workerThread,               SIGNAL(started()),      threadedFileServer, SLOT(init()));
    connect(threadedFileServer->socket, SIGNAL(disconnected()), workerThread,       SLOT(quit()));
    workerThread->start();
}

void FileServer::setDownloadFolder(const QString &value)
{
    downloadFolder = value;
}
