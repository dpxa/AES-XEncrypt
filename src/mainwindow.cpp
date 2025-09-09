#include "../header/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include "../header/EncryptionThread.h"
#include <QMessageBox>
#include <QThread>

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
    
    // throughput display
    throughputLabel = new QLabel(this);
    ui->statusbar->addPermanentWidget(throughputLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete showPath;
    delete showKeyPath;
    delete throughputLabel;
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

// updates after each file is processed (list is updated)
void MainWindow::updateThroughputDisplay() {
    double throughput = ddfs.getCurrentThroughputMBps();
    size_t totalBytes = ddfs.getTotalBytesProcessed();
    int filesProcessed = ddfs.getFilesProcessed();
    double elapsedTime = ddfs.getElapsedTimeSeconds();
    double totalMB = totalBytes / (1024.0 * 1024.0);

    throughputLabel->setText(QString("Throughput: %1 MB/s | Files: %2 | Total: %3 MB | Time: %4 s")
                            .arg(QString::number(throughput, 'f', 2))
                            .arg(filesProcessed)
                            .arg(QString::number(totalMB, 'f', 2))
                            .arg(QString::number(elapsedTime, 'f', 2)));
}

void MainWindow::on_EncryptButton_clicked()
{
    // save key file if needed
    if (newKey) {
        newKey = false;
        encryptedText.saveKey();
    }

    encryptedText.setState(false);

    ui->EncryptButton->setEnabled(false);
    ui->DecryptButton->setEnabled(false);
    ui->setDirPath->setEnabled(false);
    ui->setKeyPath->setEnabled(false);
    ui->statusbar->showMessage("Encrypting...");
    updateButtonColors();

    // return to UI thread to update file list
    auto fileCallback = [this](const QString &msg) {
        QMetaObject::invokeMethod(ui->fileNamesList, [this, msg]() {
            ui->fileNamesList->insertItem(0, msg);
            updateThroughputDisplay();
        }, Qt::QueuedConnection);
    };

    // new thread and worker
    QThread *thread = new QThread;
    EncryptionWorker *worker = new EncryptionWorker(&ddfs, &encryptedText, fileCallback);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &EncryptionWorker::process);

    connect(worker, &EncryptionWorker::finished, this, [=]() {
        ui->statusbar->showMessage("Encryption done!");
        ui->DecryptButton->setEnabled(true);
        ui->setDirPath->setEnabled(true);
        ui->setKeyPath->setEnabled(true);
        updateButtonColors();
    });

    // stop and delete thread when worker done
    connect(worker, &EncryptionWorker::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    ddfs.setEncrypted(encryptedText);
    thread->start();
}

void MainWindow::on_DecryptButton_clicked()
{
    encryptedText.setState(true);

    ui->EncryptButton->setEnabled(false);
    ui->DecryptButton->setEnabled(false);
    ui->setDirPath->setEnabled(false);
    ui->setKeyPath->setEnabled(false);
    ui->statusbar->showMessage("Decrypting...");
    updateButtonColors();

    // return to UI thread to update file list
    auto fileCallback = [this](const QString &msg) {
        QMetaObject::invokeMethod(ui->fileNamesList, [this, msg]() {
            ui->fileNamesList->insertItem(0, msg);
            updateThroughputDisplay();
        }, Qt::QueuedConnection);
    };

    // new thread and worker
    QThread *thread = new QThread;
    EncryptionWorker *worker = new EncryptionWorker(&ddfs, &encryptedText, fileCallback);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &EncryptionWorker::process);

    connect(worker, &EncryptionWorker::finished, this, [=]() {
        ui->statusbar->showMessage("Decryption done!");
        ui->EncryptButton->setEnabled(true);
        ui->setDirPath->setEnabled(true);
        ui->setKeyPath->setEnabled(true);
        updateButtonColors();
    });

    // stop and delete thread when worker done
    connect(worker, &EncryptionWorker::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    ddfs.setEncrypted(encryptedText);
    thread->start();
}

void MainWindow::on_clearFileNamesList_clicked()
{
    ui->fileNamesList->clear();
}
