#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "DirectoryDFS.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_EncryptButton_clicked();

    void on_DecryptButton_clicked();

    void on_setDirPath_clicked();

    void on_setKeyPath_clicked();

    void on_clearFileNamesList_clicked();

private:
    void updateButtonColors();

    Ui::MainWindow *ui;

    // labels for current path and key path (child of scrollBarArea)
    QLabel *showPath;
    QLabel *showKeyPath;

    DirectoryDFS ddfs;
    EncryptedText encryptedText;

    bool pathSet = false, keySet = false, newKey = false;
};

#endif // MAINWINDOW_H
