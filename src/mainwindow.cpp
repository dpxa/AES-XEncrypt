#include "../header/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include <QMessageBox> // For displaying message boxes

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // disable buttons by default
    ui->EncryptButton->setEnabled(false);
    ui->DecryptButton->setEnabled(false);
    updateButtonColors();

    showPath = new QLabel(this);
    ui->scrollAreaDirPath->setWidget(showPath);
    showKeyPath = new QLabel(this);
    ui->scrollAreaKeyPath->setWidget(showKeyPath);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete showPath;
    delete showKeyPath;
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


void MainWindow::on_setDirPath_clicked()
{
    // get valid path
    QString path = ui->dirPath->text();
    bool isValidPath = ddfs.validatePath(path.toStdString());

    ui->statusbar->showMessage(isValidPath ? "Valid path." : "Invalid path.");

    if (isValidPath) {
        // show vaid path
        pathSet = true;
        showPath->setText(path);
        ui->EncryptButton->setEnabled(keySet);
        ui->DecryptButton->setEnabled(keySet && !newKey);
        updateButtonColors();
    } else {
        QMessageBox::warning(this, "Warning", "Invalid path entered!");
    }
}

void MainWindow::on_setKeyPath_clicked()
{
    // get valid key
    QString keyPath = ui->keyPath->text();
    std::string keyPathStr = keyPath.toStdString();

    EncryptedText::KeyFileStatus status = encryptedText.validateKeyFile(keyPathStr);

    if (status == EncryptedText::KeyFileStatus::ValidKey) {
        ui->statusbar->showMessage("Valid path to key.");
        newKey = false;
    } else {
        // attempt to create a new key
        if (encryptedText.newKey(keyPathStr)) {
            newKey = true;
            if (status == EncryptedText::KeyFileStatus::InvalidKey) {
                ui->statusbar->showMessage("File is present but no valid key found - new key will be created.");
            } else {
                ui->statusbar->showMessage("No valid file found - new key will be created.");
            }
        // if not valid key file, exit
        } else {
            ui->statusbar->showMessage("Invalid key path entered.");
            QMessageBox::warning(this, "Warning", "Invalid key path entered.");
            return;
        }
    }

    // show valid key path
    keySet = true;
    showKeyPath->setText(keyPath);
    ui->EncryptButton->setEnabled(pathSet);
    ui->DecryptButton->setEnabled(pathSet && !newKey);
    updateButtonColors();
}
void MainWindow::on_EncryptButton_clicked()
{
    // only save key file after first encrypt
    if (newKey) {
        newKey = false;
        encryptedText.saveKey();
    }

    encryptedText.setState(false);
    ui->statusbar->showMessage("Encrypting...");

    ddfs.setEncrypted(encryptedText);
    ddfs.performDFS(ui->fileNamesList);

    ui->statusbar->showMessage("Encryption done!");

    // switch button states
    ui->EncryptButton->setEnabled(false);
    ui->DecryptButton->setEnabled(true);
    updateButtonColors();
}


void MainWindow::on_DecryptButton_clicked()
{
    encryptedText.setState(true);
    ui->statusbar->showMessage("Decrypting...");

    ddfs.setEncrypted(encryptedText);
    ddfs.performDFS(ui->fileNamesList);

    ui->statusbar->showMessage("Decryption done!");

    // switch button states
    ui->DecryptButton->setEnabled(false);
    ui->EncryptButton->setEnabled(true);
    updateButtonColors();
}

void MainWindow::on_clearFileNamesList_clicked()
{
    ui->fileNamesList->clear();
}

