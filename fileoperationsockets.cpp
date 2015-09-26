#include "fileoperationsockets.h"
#include <QMessageBox>
#include "utildefines.h"
#include <QFileInfo>
#include <QHostAddress>
#include <QDir>

namespace FileOperation
{

Socket::Socket() :
    file(new QFile(this)),
    totalBytes(0),
    bytesDone(0),
    progressTimer(new QTimer(this)),
    speedTimer(new QTimer(this))
{
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),  this, SLOT(reportError(QAbstractSocket::SocketError)));
    connect(progressTimer,  SIGNAL(timeout()), this, SLOT(checkUpdate()));
    connect(speedTimer,     SIGNAL(timeout()), this, SLOT(checkSpeed()));
}

Socket::~Socket()
{
    if (totalBytes > 0) {
        emit signalProgress((100.*bytesDone)/totalBytes);
        checkSpeed();
        if(bytesDone == totalBytes)
            emit signalStatus("Completed");
    }
}

void Socket::reportError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        emit signalStatus("RemoteHostClosed");
        break;
    case QAbstractSocket::HostNotFoundError:
        emit disconnected();
        emit signalStatus("HostNotFound");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit disconnected();
        emit signalStatus("ConnectionRefused");
        break;
    default:
        emit disconnected();
        emit signalStatus(errorString());
    }
}

void Socket::checkUpdate()
{
    int progressNew = (100.*bytesDone)/totalBytes;
    if (progressNew != progress) {
        progress = progressNew;
        emit signalProgress(progress);
    }
}

void Socket::checkSpeed()
{
    double deltaMB = bytesDone / 1048576.;
    double deltaS = speedElapsedTimer.elapsed() / 1000.;
    double speed = deltaMB/deltaS;
    emit signalSpeed(QString::number(speed) + "MB/s");
}

void Socket::startTimers()
{
    progressTimer->start(PROGRESSUPDATEINTERVAL);
    speedTimer->start(SPEEDUPDATEINTERVAL);
    speedElapsedTimer.start();
}

ClientSocket::ClientSocket(QString _address, QString _filePath)
{
    connect(this, SIGNAL(connected()),          this, SLOT(startTransferring()));
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten()));
    address = _address;
    filePath = _filePath;
}

void ClientSocket::onStartOperation()
{
    file->setFileName(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "Client: file open error";
        return;
    }

    emit signalStatus("Connecting");
    connectToHost(address, PORT);
}

void ClientSocket::startTransferring()
{
    QDataStream out(this);
    out.setVersion(QDataStream::Qt_4_0);

    QFileInfo fileInfo(*file);
    QString fileNameString = fileInfo.fileName();
    QByteArray fileName = fileNameString.toUtf8();
    qint64 fileNameLength = fileName.size();

    totalBytes = file->size();
    bytesToDo = totalBytes;

    emit signalStatus("Uploading");

    out << fileNameLength;
    write(fileName);
    out << totalBytes;

    startTimers();
}

void ClientSocket::onBytesWritten()
{
    if (bytesToDo > 0 && bytesToWrite() <= 4*PAYLOADSIZE) {
        qint64 count = write(file->read(PAYLOADSIZE));
        bytesToDo -= count;
        bytesDone += count;
    }
}

ServerSocket::ServerSocket(qintptr socketDescriptor, QString downloadFolder) :
    socketDescriptor(socketDescriptor),
    downloadFolder(downloadFolder),
    dataRead(false),
    fileNameLength(0),
    fileName(QString())
{
    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void ServerSocket::onStartOperation()
{
    emit signalStatus("Initializing");
    if (!setSocketDescriptor(socketDescriptor, QAbstractSocket::ConnectedState, QIODevice::ReadOnly)) {
        emit reportError(error());
        cleanUp();
    }
}

void ServerSocket::onReadyRead()
{
    // Header Data
    if (!dataRead) {
        QDataStream in(this);
        in.setVersion(QDataStream::Qt_4_0);

        if (fileNameLength == 0) {
            if (bytesAvailable() < (qint64)sizeof(qint64))
                return;
            in >> fileNameLength;
        }

        if (fileName.isEmpty()) {
            if (bytesAvailable() < fileNameLength)
                return;
            fileName = QString::fromUtf8(read(fileNameLength));
            emit signalFileName(fileName);

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
            if (bytesAvailable() < (qint64)sizeof(qint64))
                return;
            in >> totalBytes;
            dataRead = true;
            address = peerAddress().toString();
            emit signalAddress(address);
            emit signalFileSize(QString::number( (double)totalBytes / 1048576.) + "MB");
            emit signalStatus("Downloading");
            startTimers();
        }
    }

    // Data
    bytesDone += bytesAvailable();
    file->write(readAll());

    if (bytesDone == totalBytes) {
        cleanUp();
    }
}

void ServerSocket::cleanUp()
{
    close();
    file->close();
}

} //namespace
