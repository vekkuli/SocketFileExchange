#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include "fileserver.h"
#include "fileexchangetablemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_downloadFolderBrowseButton_clicked();
    void on_downloadFolderLineEdit_editingFinished();
    void on_fileSystemTree_doubleClicked(const QModelIndex &index);
    void checkAcceptConnections();

private:

    Ui::MainWindow *ui;
    QFileSystemModel*       fileSystemModel;
    FileServer*             fileServer;
    FileExchange::UploadsTableModel*    uploadsTableModel;
    FileExchange::DownloadsTableModel*  downloadsTableModel;
};

#endif // MAINWINDOW_H
