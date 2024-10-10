#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_valDirPath_clicked();

    void on_yesKey_clicked();

    void on_noKey_clicked();

    void on_dirKeyPath_clicked();

    void on_YesEnc_clicked();

    void on_NoEnc_clicked();

private:
    Ui::MainWindow *ui;
    DirectoryDFS ddfs;
    EncryptedText encryptedText;
    bool hasKey = false;
    bool newKey = false;
    bool pathSet = false, keySet = false;
};
#endif // MAINWINDOW_H
