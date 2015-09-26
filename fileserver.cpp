#include "fileserver.h"

FileServer::FileServer(QObject *parent)
    : QTcpServer(parent)
{
}

void FileServer::incomingConnection(qintptr socketDescriptor)
{
    emit signalIncomingConnection(socketDescriptor, downloadFolder);
}

void FileServer::setDownloadFolder(const QString &value)
{
    downloadFolder = value;
}
