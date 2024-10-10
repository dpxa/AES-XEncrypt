#include "../header/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include <QMessageBox> // For displaying message boxes
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->YesEnc->setEnabled(false);
    ui->NoEnc->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_valDirPath_clicked()
{
    QString path = ui->dirPath->text();
    if (ddfs.validatePath(path.toStdString())) {
        ui->statusbar->showMessage("Valid path: " + path);
        pathSet = true;
        ui->YesEnc->setEnabled(pathSet && keySet);
        ui->NoEnc->setEnabled(pathSet && keySet && !newKey);
    } else {
        ui->statusbar->showMessage("Invalid path: " + path);
        QMessageBox::warning(this, "Warning", "Invalid path entered!");
    }
}


void MainWindow::on_yesKey_clicked()
{
    hasKey = true;
    ui->statusbar->showMessage("A key file must be provided for this path.");
}


void MainWindow::on_noKey_clicked()
{
    hasKey = false;
    ui->statusbar->showMessage("No key file found. A new key file will be generated.");
}


void MainWindow::on_dirKeyPath_clicked()
{
    QString keyPath = ui->keyPath->text();

    if (hasKey) {
        if (encryptedText.validateKeyFile(keyPath.toStdString())) {
            ui->statusbar->showMessage("Valid key path: " + keyPath);
            keySet = true;
            newKey = false;
            ui->YesEnc->setEnabled(pathSet && keySet);
            ui->NoEnc->setEnabled(pathSet && keySet && !newKey);
        } else {
            ui->statusbar->showMessage("Invalid key path: " + keyPath);
            QMessageBox::warning(this, "Warning", "Invalid key path entered!");
        }
    } else {
        if (encryptedText.createKeyFile(keyPath.toStdString())) {
            ui->statusbar->showMessage("Valid key path: " + keyPath);
            keySet = true;
            newKey = true;
            ui->YesEnc->setEnabled(pathSet && keySet);
            ui->NoEnc->setEnabled(pathSet && keySet && !newKey);
        } else {
            ui->statusbar->showMessage("Invalid key path: " + keyPath);
            QMessageBox::warning(this, "Warning", "Invalid key path entered!");
        }
    }
}


void MainWindow::on_YesEnc_clicked()
{
    encryptedText.setState(true);
    ui->fileNames->clear();
    ui->statusbar->showMessage("Encrypting...");
    ddfs.setEncrypted(encryptedText);
    ddfs.performDFS(ui->fileNames);
    ui->statusbar->showMessage("Encryption done!");

    if (!newKey) {
        newKey = false;
        ui->NoEnc->setEnabled(pathSet && keySet && !newKey);
    }
}


void MainWindow::on_NoEnc_clicked()
{
    encryptedText.setState(false);
    ui->fileNames->clear();
    ui->statusbar->showMessage("Decrypting...");
    ddfs.setEncrypted(encryptedText);
    ddfs.performDFS(ui->fileNames);
    ui->statusbar->showMessage("Decryption done!");
}

