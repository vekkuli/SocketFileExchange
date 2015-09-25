#ifndef FILEEXCHANGEITEM_H
#define FILEEXCHANGEITEM_H

#include <QObject>
#include <QModelIndex>
#include <QVariant>
#include "fileoperationsockets.h"

class FileExchangeItem : public QObject
{
    Q_OBJECT
public:
    explicit FileExchangeItem(QObject* parent = 0, QString address = 0, QString filePath = 0);

    QVariant data(int column);
    void setRow(int value);

signals:
    void thisChanged(int row, int col1, int col2);

public slots:
    void infoChanged(QString _fileName, QString _address, QString _fileSize);
    void progressChanged(int _progress);
    void statusChanged(QString _status);
    void speedChanged(QString _speed);

protected:
    QString address;
    QString fileName;
    QString fileSize;
    int progress;
    QString speed;
    QString status;
    int row;
};

#endif // UPLOADITEM_H
