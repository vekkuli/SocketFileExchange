#ifndef UPLOADITEM_H
#define UPLOADITEM_H

#include "fileexchangeitem.h"
#include <QThread>


class UploadItem : public FileExchangeItem
{
    Q_OBJECT

public:
    UploadItem(QObject* parent, QString _address, QString filePath);
    ~UploadItem();

signals:
    void startUpload();
    void finished();

public slots:

private:
    QThread* workerThread;
};

#endif // UPLOADITEM_H
