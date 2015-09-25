#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QTcpSocket>
#include <QTimer>
#include <QElapsedTimer>
#include <QFile>

namespace FileOperation
{

class Socket : public QTcpSocket
{
    Q_OBJECT

public:
    Socket();
    ~Socket();

public slots:
    void reportError(QAbstractSocket::SocketError socketError);

private slots:   
    void checkUpdate();
    void checkSpeed();

signals:
    void signalInfo(QString, QString, QString);
    void signalProgress(int);
    void signalStatus(QString);
    void signalSpeed(QString);

protected:
    void startTimers();

    QString address;
    QString filePath;
    QFile* file;
    qint64 totalBytes;
    qint64 bytesDone;

private:
    QTimer* progressTimer;
    int progress;
    QTimer* speedTimer;
    QElapsedTimer speedElapsedTimer;
};

class ClientSocket : public Socket
{
    Q_OBJECT

public:
    ClientSocket(QString _address, QString _filePath);

public slots:
    void initTransfer();
    void startTransferring();
    void onBytesWritten();

private:
    qint64 bytesToDo;
};

class ServerSocket : public Socket
{
    Q_OBJECT

public:
    ServerSocket(int socketDescriptor, QString downloadFolder);

public slots:
    void init();
    void onReadyRead();

signals:
    void done();

private:
    void cleanUp();

    int socketDescriptor;
    QString downloadFolder;
    bool dataRead;
    qint64 fileNameLength;
    QString fileName;
};

//class ThreadedFileClient : public QObject
//{
//    Q_OBJECT

//public:
//    ThreadedFileClient(QString address, QString filePath);
//    ~ThreadedFileClient();

//    QTcpSocket *socket;

//public slots:
//    void initTransfer();

//private slots:
//    void startTransferring();
//    void updateProgress();
//    void displayError(QAbstractSocket::SocketError socketError);
//    void checkUpdate();
//    void checkSpeed();

//signals:
//    void signalInfo(QString, QString, QString);
//    void signalProgress(int);
//    void signalStatus(QString);
//    void signalSpeed(QString);

//private:
//    QString address;
//    QString filePath;

//    QFile* file;
//    qint64 totalBytes;
//    qint64 bytesToWrite;
//    qint64 bytesWritten;

//    QTimer* progressTimer;
//    int progress;

//    QTimer* speedTimer;
//    QElapsedTimer speedElapsedTimer;
//};

} // namespace
#endif
