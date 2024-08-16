#include "videoread.h"
#include<QMessageBox>
#include<QDebug>


VideoRead::VideoRead(QObject *parent) : QObject(parent),m_hat(":/images/hat.png"),m_tuer(":/images/tuer.png")
{
    m_timer = new QTimer;
    //connect( m_timer , SIGNAL(timeout()), this , SLOT( slot_getVideoFrame()) );

    m_pVideoWorker = QSharedPointer<VideoWorker>(new VideoWorker);
    m_pVideoWorker.data()->slot_setInfo(this);
    connect(m_timer,SIGNAL(timeout()),m_pVideoWorker.data(),SLOT(slot_dowork()));

    MyFaceDetect::FaceDetectInit();

    //加载萌拍的图片
    m_moji = 0;

}

VideoRead::~VideoRead()
{
    if( m_timer )
    {
        m_timer->stop();
        delete m_timer; m_timer = NULL;
    }
}

//摄像头采集图像流程
void VideoRead::slot_getVideoFrame()
{
    //从摄像头对象读取数据
    Mat frame; // Mat 对象存储图像
    if( !cap.read(frame) )
    {
        return;
    }
    // Mat 图像不是显示和传输的格式(RGB)
    // 需要格式转换 opencv 采集的格式BGR  显示格式 RGB
    //将opencv采集的BGR的图片类型转化为RGB24的类型
    // Mat 图像不是显示和传输的格式(RGB)
    //需要格式转换 opencv 采集的格式BGR显示格式 RGB
    //将opencv采集的BGR的图片类型转化为RGB24的类型
    // 加滤镜 加人脸识别，加萌拍..· opencv 图像处理 todo
    //1获取摄像头图片后 识别出人脸的位置，返回位置对应的矩形框
    QImage tmpimg;
    switch(m_moji){
    case moji_hat:
        tmpimg = m_hat;
        break;
    case moji_tuer:
        tmpimg=m_tuer;
        break;
    }

    std::vector<Rect> faces;
    if(m_moji!=0){
        MyFaceDetect::detectAndDisplay(frame,faces);
    }


    cvtColor(frame,frame,CV_BGR2RGB);
    //定义QImage对象, 用于发送数据以及图片显示
    QImage image((unsigned const char*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888);

    //将道具放置在人脸位置
    if(faces.size()>0)
    {
        m_vecFace=faces;
    }
    if(m_moji == moji_hat||m_moji == moji_tuer){
        QPainter paint(&image);
        for(int i=0;i<m_vecFace.size();i++){
            Rect rct = m_vecFace[i];
            int x = rct.x + rct.width*0.5 - tmpimg.width()*0.5 + 20;
            int y = rct.y-tmpimg.height();
            QPoint p(x,y);
            paint.drawImage(p,tmpimg);
        }
    }
    //转化为大小更小的图片  保持缩放比
    image = image.scaled( IMAGE_WIDTH,IMAGE_HEIGHT, Qt::KeepAspectRatio );
    //发送图片
    Q_EMIT SIG_sendVideoFrame( image );

}

void VideoRead::slot_openVideo()
{
    m_timer->start( 1000/FRAME_RATE - 10 );
    //打开摄像头
    cap.open(0); // 0 默认摄像头
    if(!cap.isOpened()){
        QMessageBox::information(NULL,tr("提示"),tr("视频没有打开"));
        return;
    }

}

void VideoRead::slot_closeVideo()
{
    m_timer->stop();
    //关闭摄像头
    if(cap.isOpened())
        cap.release();
}

void VideoRead::slot_setMoji(int newMoji)
{
    m_moji = newMoji;
}
