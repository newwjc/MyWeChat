#ifndef SCREENREAD_H
#define SCREENREAD_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include<QApplication>
#include<QDesktopWidget>
#include<QBuffer>
#include<QScreen>
#include "common.h"

class ScreenRead : public QObject
{
    Q_OBJECT
public:
    explicit ScreenRead(QObject *parent = nullptr);
    ~ScreenRead();

signals:
    //将获取到的桌面信息发送出去
    void SIG_getScreenFrame(QImage img);

public slots:
    void slot_getScreenFrame();//定时获取画面

    void slot_openVideoScreen();
    void slot_closeVideoScreen();
private:
    QTimer* m_timer;//定时器，用于定时获取画面

};

#endif // SCREENREAD_H
