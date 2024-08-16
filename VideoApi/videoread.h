#ifndef VIDEOREAD_H
#define VIDEOREAD_H

#include <QObject>
#include <QImage>
#include<QTimer>
#include <QImage>
#include <QPainter>
#include"common.h"
#include "threadworker.h"
#include "myfacedetect.h"
//图片的宽高
#define IMAGE_WIDTH (320)
#define IMAGE_HEIGHT (240)

class VideoWorker;
class VideoRead : public
        QObject
{
    Q_OBJECT
public:
    explicit VideoRead(QObject *parent = nullptr);
    ~VideoRead();


signals:
    void SIG_sendVideoFrame(QImage img);
public slots:
    void slot_setMoji(int newMoji);
    void slot_getVideoFrame();
    void slot_openVideo();
    void slot_closeVideo();
private:
    QTimer *m_timer;

    //opencv获取图片的对象
    cv::VideoCapture cap;
    //定义多线程对象
    QSharedPointer<VideoWorker> m_pVideoWorker;

    std::vector<Rect> m_vecFace;

    enum moji_type{moji_hat =1,moji_tuer};
    int m_moji;
    QImage m_hat;
    QImage m_tuer;

};
class VideoWorker:public ThreadWorker{
    Q_OBJECT
public slots:
    void slot_setInfo(VideoRead * p){
        m_pVideoRead =p;
    }
    //定时器到时执行
    void slot_dowork(){
        m_pVideoRead->slot_getVideoFrame();
    }

private:
    VideoRead * m_pVideoRead;
};

#endif // VIDEOREAD_H
