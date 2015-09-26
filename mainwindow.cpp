#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "utildefines.h"
#include "fileexchangeitemdelegate.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileSystemModel(new QFileSystemModel(this)),
    uploadsTableModel(new FileExchange::UploadsTableModel(this)),
    downloadsTableModel(new FileExchange::DownloadsTableModel(this)),
    fileServer(new FileServer(this))
{
    ui->setupUi(this);
    connect(ui->acceptConnectionsCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkAcceptConnections()));

    // Settings
    QSettings settings;
    ui->downloadFolderLineEdit->setText(settings.value("downloadFolder", QDir::home().absolutePath()).toString());
    ui->acceptConnectionsCheckBox->setChecked(settings.value("acceptConnections", false).toBool());
    ui->addressLineEdit->setText(settings.value("address", "localhost").toString());

    // File System
    fileSystemModel->setRootPath(QDir::currentPath());
    ui->fileSystemTree->setModel(fileSystemModel);
    ui->fileSystemTree->header()->setStretchLastSection(false);
    ui->fileSystemTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    // Uploads
    ui->uploadsTable->setModel(uploadsTableModel);
    ui->uploadsTable->setItemDelegate(new FileExchangeItemDelegate(this));
    ui->uploadsTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->uploadsTable->setColumnWidth(0, 160);
    ui->uploadsTable->setColumnWidth(2, 80);
    ui->uploadsTable->setColumnWidth(3, 80);

    // Downloads
    ui->downloadsTable->setModel(downloadsTableModel);
    ui->downloadsTable->setItemDelegate(new FileExchangeItemDelegate(this));
    ui->downloadsTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->downloadsTable->setColumnWidth(0, 160);
    ui->downloadsTable->setColumnWidth(2, 80);
    ui->downloadsTable->setColumnWidth(3, 80);   

    // File Server (downloads)
    fileServer->setDownloadFolder(ui->downloadFolderLineEdit->text());
    connect(fileServer, SIGNAL(signalIncomingConnection(qintptr,QString)), downloadsTableModel, SLOT(addNewDownload(qintptr,QString)));
    checkAcceptConnections();
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("downloadFolder", ui->downloadFolderLineEdit->text());
    settings.setValue("acceptConnections", ui->acceptConnectionsCheckBox->isChecked());
    settings.setValue("address", ui->addressLineEdit->text());
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

void MainWindow::checkAcceptConnections()
{
    if (ui->acceptConnectionsCheckBox->isChecked()) {
        if (!fileServer->isListening()) {
            if (!fileServer->listen(QHostAddress::Any, PORT)) {
                QMessageBox::critical(this, tr("FileServer"),
                                      tr("Unable to start the server: %1.")
                                      .arg(fileServer->errorString()));
            }
        }
    } else {
        fileServer->close();
    }
}
