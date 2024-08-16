#include "screenread.h"

ScreenRead::ScreenRead(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer;
    connect(m_timer,SIGNAL(timeout()),
            this,SLOT( slot_getScreenFrame() ));
}

ScreenRead::~ScreenRead()
{
    if(m_timer != nullptr)
    {
        m_timer->stop();
        delete  m_timer;
        m_timer = nullptr;
    }
}

void ScreenRead::slot_getScreenFrame()
{
    //获取桌面对象
    QScreen *src = QApplication::primaryScreen();
    //获取当前分辨率，得到桌面矩形
    //QRect deskRect = QApplication::desktop()->screenGeometry();

    //获取桌面图片
    QPixmap map = src->grabWindow( QApplication::desktop()->winId()/* ,0,0 , deskRect.width() ,
                                   deskRect.height()*/);
    QImage image = map.toImage();
    //缩放，改变尺寸
    // image = image.scaled( 1600, 900, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    Q_EMIT SIG_getScreenFrame(image);
    /*
    //压缩图片从 RGB24 格式压缩到 JPEG 格式, 发送出去
    QByteArray ba;
    QBuffer qbuf(&ba); // QBuffer 与 QByteArray 字节数组联立联系
    image.save( &qbuf , "JPEG" , 60 ); //将图片的数据写入 ba
    Q_EMIT SIG_screenFrameData( ba );
    */
}

void ScreenRead::slot_openVideoScreen()
{
    m_timer->start(1000 / FRAME_RATE -10);
}

void ScreenRead::slot_closeVideoScreen()
{
    m_timer->stop();
}
