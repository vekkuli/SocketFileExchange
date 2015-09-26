#ifndef FILEEXCHANGETABLEMODEL_H
#define FILEEXCHANGETABLEMODEL_H

#include <QAbstractTableModel>
#include <QQueue>
#include <QList>
#include "fileexchangeitem.h"

namespace FileExchange
{

class BaseTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    BaseTableModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

public slots:
    void itemChanged(int row, int col1, int col2);
    virtual void itemFinished(int row) = 0;

protected:
    int addItem(FileExchangeItem* newItem);

    QList<FileExchangeItem*> items;
};

class UploadsTableModel : public BaseTableModel
{
    Q_OBJECT

public:
    UploadsTableModel(QObject* parent = 0);
    void addNewUpload(QString address, QString filePath);

public slots:
    void itemFinished(int row);

private:
    void checkQueue();

    QQueue<FileExchangeItem*> itemsQueue;
    bool busy;
};

class DownloadsTableModel : public BaseTableModel
{
    Q_OBJECT

public:
    DownloadsTableModel(QObject* parent = 0);

signals:

public slots:
    void addNewDownload(qintptr socketDescriptor, QString downloadFolder);
    void itemFinished(int row);

private:
};

} // namespace

#endif // UPLOADTABLEMODEL_H
