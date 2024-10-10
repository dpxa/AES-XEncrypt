#include "../header/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include <QMessageBox> // For displaying message boxes

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    } else {
        ui->statusbar->showMessage("Invalid path: " + path);
        QMessageBox::warning(this, "Warning", "Invalid path entered!");
    }
}


void MainWindow::on_yesKey_clicked()
{
    hasKey = true;
    ui->statusbar->showMessage("A key is present for this path.");
}


void MainWindow::on_noKey_clicked()
{
    hasKey = false;
    ui->statusbar->showMessage("A key needs to be made.");
}


void MainWindow::on_dirKeyPath_clicked()
{
    QString keyPath = ui->keyPath->text();

    if (hasKey) {
        if (encryptedText.validateKeyFile(keyPath.toStdString())) {
            ui->statusbar->showMessage("Valid key path: " + keyPath);
            keySet = true;
        } else {
            ui->statusbar->showMessage("Invalid key path: " + keyPath);
            QMessageBox::warning(this, "Warning", "Invalid key path entered!");
        }
    } else {
        if (encryptedText.createKeyFile(keyPath.toStdString())) {
            ui->statusbar->showMessage("Valid key path: " + keyPath);
            keySet = true;
            newKey = true;
        } else {
            ui->statusbar->showMessage("Invalid key path: " + keyPath);
            QMessageBox::warning(this, "Warning", "Invalid key path entered!");
        }
    }
}


void MainWindow::on_YesEnc_clicked()
{
    if (pathSet && keySet) {
        encryptedText.setState(true);
        ui->statusbar->showMessage("Encrypting...");
        ddfs.setEncrypted(encryptedText);
        ddfs.performDFS();
        ui->statusbar->showMessage("Encryption done!");
        newKey = false;
    }
}


void MainWindow::on_NoEnc_clicked()
{
    if (newKey) {
        ui->statusbar->showMessage("Cannot decrypt file with new key");
        QMessageBox::warning(this, "Warning", "Cannot decrypt file with new key");
    } else if (pathSet && keySet) {
        encryptedText.setState(false);
        ui->statusbar->showMessage("Decrypting...");
        ddfs.setEncrypted(encryptedText);
        ddfs.performDFS();
        ui->statusbar->showMessage("Decryption done!");
    } else if (!pathSet && !keySet) {
        ui->statusbar->showMessage("Path and key not set");
        QMessageBox::warning(this, "Warning", "Path and key not set");
    } else if (pathSet) {
        ui->statusbar->showMessage("Key not set");
        QMessageBox::warning(this, "Warning", "Key not set");
    } else {
        ui->statusbar->showMessage("Path not set");
        QMessageBox::warning(this, "Warning", "Path not set");
    }
}
