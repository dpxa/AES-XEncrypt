#ifndef ENCRYPTIONWORKER_H
#define ENCRYPTIONWORKER_H

#include <QObject>
#include "DirectoryDFS.h"
#include "Encrypted.h"

class EncryptionWorker : public QObject {
    Q_OBJECT
public:
    explicit EncryptionWorker(DirectoryDFS *ddfs, EncryptedText *encryptedText, std::function<void(const QString&)> fileCallback, QObject *parent = nullptr)
        : QObject(parent), m_ddfs(ddfs), m_encryptedText(encryptedText), callBack(fileCallback) {}

signals:
    void finished();

public slots:
    void process() {
        m_ddfs->performDFS(callBack);
        emit finished();
    }

private:
    DirectoryDFS *m_ddfs;
    EncryptedText *m_encryptedText;
    std::function<void(const QString&)> callBack;
};

#endif // ENCRYPTIONWORKER_H
