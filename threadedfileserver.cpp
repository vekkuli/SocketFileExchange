#include "threadedfileserver.h"
#include <QDebug>
#include <QDataStream>
#include <QDir>

ThreadedFileServer::ThreadedFileServer(int socketDescriptor, QString downloadFolder)
    : socket(new QTcpSocket(this)),
      socketDescriptor(socketDescriptor),
      file(new QFile(this)),
      totalBytes(0),
      bytesReceived(0),
      downloadFolder(downloadFolder),
      dataRead(false),
      fileNameLength(0),
      fileName(QString())
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
}

void ThreadedFileServer::init()
{
    timer.restart();  
    if (!socket->setSocketDescriptor(socketDescriptor, QAbstractSocket::ConnectedState, QIODevice::ReadOnly)) {
        emit error(socket->error());
        cleanUp();
    }
}

void ThreadedFileServer::read()
{
    // Header Data
    if (!dataRead) {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_4_0);

        if (fileNameLength == 0) {
            if (socket->bytesAvailable() < (qint64)sizeof(qint64))
                return;
            in >> fileNameLength;
        }

        if (fileName.isEmpty()) {
            if (socket->bytesAvailable() < fileNameLength)
                return;
            fileName = QString::fromUtf8(socket->read(fileNameLength));

            // Open File
            file->setFileName(QDir::cleanPath(downloadFolder) + "/" + fileName);

            if (!file->open(QIODevice::WriteOnly))
            {
                qDebug() << "Server: File Open Error" << file->errorString() << file->fileName();
                cleanUp();
                return;
            }
        }

        if (totalBytes == 0) {
            if (socket->bytesAvailable() < (qint64)sizeof(qint64))
                return;
            in >> totalBytes;
            dataRead = true;
        }
    }

    // Data
    bytesReceived += socket->bytesAvailable();
    file->write(socket->readAll());

    if (bytesReceived == totalBytes) {
        qDebug() << tr("Server: %1 Downloaded").arg(file->fileName()) << ((double)totalBytes/(1024.*1024.))/((double)timer.elapsed()/1000.) << "MB/s";
        cleanUp();
    }
}

void ThreadedFileServer::cleanUp()
{
    socket->close();
    file->close();

    emit done();
}
