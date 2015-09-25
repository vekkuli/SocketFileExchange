#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "utildefines.h"
#include "fileexchangeitemdelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileSystemModel(new QFileSystemModel(this)),
    uploadsTableModel(new UploadsTableModel(this)),
    downloadsTableModel(new FileExchangeTableModel(this)),
    fileServer(new FileServer(this))
{
    ui->setupUi(this);

    if (!fileServer->listen(QHostAddress::Any, PORT)) {
        QMessageBox::critical(this, tr("FileServer"),
                              tr("Unable to start the server: %1.")
                              .arg(fileServer->errorString()));
        close();
        return;
    }

    fileSystemModel->setRootPath(QDir::currentPath());
    ui->fileSystemTree->setModel(fileSystemModel);
    ui->fileSystemTree->header()->setStretchLastSection(false);
    ui->fileSystemTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);


    ui->downloadsTable->setModel(downloadsTableModel);
    ui->downloadsTable->setItemDelegate(new FileExchangeItemDelegate(this));
    ui->downloadsTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->downloadsTable->setColumnWidth(0, 160);
    ui->downloadsTable->setColumnWidth(2, 80);
    ui->downloadsTable->setColumnWidth(3, 80);

    ui->uploadsTable->setModel(uploadsTableModel);
    ui->uploadsTable->setItemDelegate(new FileExchangeItemDelegate(this));
    ui->uploadsTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->uploadsTable->setColumnWidth(0, 160);
    ui->uploadsTable->setColumnWidth(2, 80);
    ui->uploadsTable->setColumnWidth(3, 80);



//    ui->uploadsTable->horizontalHeader()->setStretchLastSection(false);
//    ui->uploadsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
//    ui->uploadsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    fileServer->setDownloadFolder(ui->downloadFolderLineEdit->text());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_downloadFolderBrowseButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    if (dialog.exec()) {
        ui->downloadFolderLineEdit->setText(dialog.selectedFiles().at(0));
        fileServer->setDownloadFolder(ui->downloadFolderLineEdit->text());
    }
}

void MainWindow::on_downloadFolderLineEdit_editingFinished()
{
    fileServer->setDownloadFolder(ui->downloadFolderLineEdit->text());
}

void MainWindow::on_fileSystemTree_doubleClicked(const QModelIndex &index)
{
    if (!fileSystemModel->isDir(index)) {
        uploadsTableModel->addNewUpload(ui->addressLineEdit->text(), fileSystemModel->filePath(index));
    }
}
