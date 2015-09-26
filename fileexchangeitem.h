#ifndef FILEEXCHANGEITEM_H
#define FILEEXCHANGEITEM_H

#include <QObject>
#include <QModelIndex>
#include <QVariant>
#include <QThread>
#include "fileoperationsockets.h"

class FileExchangeItem : public QObject
{
    Q_OBJECT
public:
    explicit FileExchangeItem(QObject* parent, FileOperation::Socket *socket, QString _status = QString(), QString _address = QString(), QString _filePath = QString());
    ~FileExchangeItem();

    QVariant data(int column);
    void setRow(int value);
    int getRow() const;

signals:
    void thisChanged(int row, int col1, int col2);
    void finished(int row);
    void startOperation();

public slots:
    void addressChanged(QString value);
    void fileNameChanged(QString value);
    void fileSizeChanged(QString value);
    void progressChanged(int value);
    void statusChanged(QString value);
    void speedChanged(QString value);
    void onSocketDisconnected();

protected:
    QThread* workerThread;
    FileOperation::Socket* socket;

    QString address;
    QString fileName;
    QString fileSize;
    int progress;
    QString speed;
    QString status;
    int row;
};

#endif // UPLOADITEM_H
