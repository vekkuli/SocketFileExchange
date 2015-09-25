#ifndef UPLOADSTABLEMODEL_H
#define UPLOADSTABLEMODEL_H

#include <QQueue>
#include "fileexchangetablemodel.h"
#include "uploaditem.h"

class UploadsTableModel : public FileExchangeTableModel
{
    Q_OBJECT

public:
    UploadsTableModel(QObject* parent = 0);

    void addNewUpload(QString address, QString filePath);


signals:

public slots:
    void uploadFinished();


private:
    void checkQueue();

    QQueue<UploadItem*> itemsQueue;
    bool busy;
};

#endif // UPLOADSTABLEMODEL_H
