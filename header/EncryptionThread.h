#ifndef ENCRYPTIONTHREAD_H
#define ENCRYPTIONTHREAD_H

#include <QObject>
#include "DirectoryDFS.h"

class EncryptionWorker : public QObject {
    Q_OBJECT
public:
    explicit EncryptionWorker(DirectoryDFS *ddfs, std::function<void(const QString&)> fileCallback, QObject *parent = nullptr)
        : QObject(parent), m_ddfs(ddfs), callBack(fileCallback) {}

signals:
    void finished();

public slots:
    void process() {
        m_ddfs->performDFS(callBack);
        emit finished();
    }

private:
    DirectoryDFS *m_ddfs;
    std::function<void(const QString&)> callBack;
};

#endif // ENCRYPTIONTHREAD_H
