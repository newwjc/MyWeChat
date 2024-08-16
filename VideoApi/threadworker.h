#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QObject>
#include <QThread>
#include <QSharedPointer>//智能指针头文件

class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = nullptr);
    virtual ~ThreadWorker();
signals:

private:
    QThread * m_thread;
};

#endif // THREADWORKER_H
