#include "../header/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include <QMessageBox> // For displaying message boxes

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->EncryptButton->setEnabled(false);
    ui->DecryptButton->setEnabled(false);
    updateButtonColors();

    ui->scrollAreaPath->setWidget(ui->showPath);
    ui->scrollAreaKeyPath->setWidget(ui->showKeyPath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateButtonColors() {
    if (ui->EncryptButton->isEnabled()) {
        ui->EncryptButton->setStyleSheet("background-color: green; color: lightyellow; border: 2px solid darkgreen;");
    } else {
        ui->EncryptButton->setStyleSheet("background-color: lightgray; color: black; border: 2px solid gray;");
    }

    if (ui->DecryptButton->isEnabled()) {
        ui->DecryptButton->setStyleSheet("background-color: red; color: lightyellow; border: 2px solid darkred;");
    } else {
        ui->DecryptButton->setStyleSheet("background-color: lightgray; color: black; border: 2px solid gray;");
    }
}


void MainWindow::on_valDirPath_clicked()
{
    QString path = ui->dirPath->text();
    bool isValidPath = ddfs.validatePath(path.toStdString());

    ui->statusbar->showMessage(isValidPath ? "Valid path." : "Invalid path.");

    if (isValidPath) {
        pathSet = true;
        ui->showPath->setText(path);
        ui->EncryptButton->setEnabled(keySet);
        ui->DecryptButton->setEnabled(keySet && !newKey);
        updateButtonColors();
    } else {
        QMessageBox::warning(this, "Warning", "Invalid path entered!");
    }
}


void MainWindow::on_dirKeyPath_clicked()
{
    QString keyPath = ui->keyPath->text();
    bool isValid = false;

    if (encryptedText.validateKeyFile(keyPath.toStdString())) {
        ui->statusbar->showMessage("Valid path to key.");
        newKey = false;
        isValid = true;
    } else if (encryptedText.newKey(keyPath.toStdString())) {
        ui->statusbar->showMessage("No valid key found - new key will be created.");
        newKey = true;
        isValid = true;
    }

    if (isValid) {
        keySet = true;
        ui->showKeyPath->setText(keyPath);
        ui->EncryptButton->setEnabled(pathSet);
        ui->DecryptButton->setEnabled(pathSet && !newKey);
        updateButtonColors();
    } else {
        ui->statusbar->showMessage("Invalid key path entered.");
        QMessageBox::warning(this, "Warning", "Invalid key path entered.");
    }
}

void MainWindow::on_EncryptButton_clicked()
{
    if (newKey) {
        newKey = false;
        ui->DecryptButton->setEnabled(true);
        updateButtonColors();
        encryptedText.saveKeyToFile();
    }

    encryptedText.setState(true);
    ui->statusbar->showMessage("Encrypting...");

    ddfs.setEncrypted(encryptedText);
    ddfs.performDFS(ui->fileNames);

    ui->statusbar->showMessage("Encryption done!");
}


void MainWindow::on_DecryptButton_clicked()
{
    encryptedText.setState(false);
    ui->statusbar->showMessage("Decrypting...");

    ddfs.setEncrypted(encryptedText);
    ddfs.performDFS(ui->fileNames);

    ui->statusbar->showMessage("Decryption done!");
}
