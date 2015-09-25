#ifndef FILEEXCHANGETABLEMODEL_H
#define FILEEXCHANGETABLEMODEL_H

#include <QAbstractTableModel>
#include "fileexchangeitem.h"

class FileExchangeTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FileExchangeTableModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

public slots:
    void itemChanged(int row, int col1, int col2);

protected:
    QList<FileExchangeItem*> items;
};

#endif // UPLOADTABLEMODEL_H
