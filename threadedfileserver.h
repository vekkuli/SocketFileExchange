#ifndef FILESERVERTHREAD_H
#define FILESERVERTHREAD_H

#include <QTcpSocket>
#include <QFile>
#include <QElapsedTimer>
#include <QString>

class ThreadedFileServer : public QObject
{
    Q_OBJECT

public:
    ThreadedFileServer(int socketDescriptor, QString downloadFolder);
    QTcpSocket* socket;

public slots:
    void init();
    void read();

signals:
    void done();
    void error(QTcpSocket::SocketError socketError);

private:
    void cleanUp();

    int socketDescriptor;

    QFile* file;
    qint64 totalBytes;
    qint64 bytesReceived;
    QElapsedTimer timer;
    QString downloadFolder;
    bool dataRead;
    qint64 fileNameLength;
    QString fileName;
};

#endif
