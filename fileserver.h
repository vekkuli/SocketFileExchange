#ifndef FILESERVER_H
#define FILESERVER_H

#include <QTcpServer>
#include <QString>

class FileServer : public QTcpServer
{
    Q_OBJECT

public:
    FileServer(QObject *parent = 0);

    void setDownloadFolder(const QString &value);

signals:
    void signalIncomingConnection(qintptr socketDescriptor, QString downloadFolder);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private:
    QString downloadFolder;
};

#endif
