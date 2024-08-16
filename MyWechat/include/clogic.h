#ifndef CLOGIC_H
#define CLOGIC_H

#include"TCPKernel.h"

class CLogic
{
public:
    CLogic( TcpKernel* pkernel )
    {
        m_pKernel = pkernel;
        m_sql = pkernel->m_sql;
        m_tcp = pkernel->m_tcp;
    }
public:
    //设置协议映射
    void setNetPackMap();
    /************** 发送数据*********************/
    void SendData( sock_fd clientfd, char*szbuf, int nlen )
    {
        m_pKernel->SendData( clientfd ,szbuf , nlen );
    }
    /************** 网络处理 *********************/
    //注册请求
    void RegisterRq(sock_fd clientfd, char*szbuf, int nlen);
    //登录请求
    void LoginRq(sock_fd clientfd, char*szbuf, int nlen);
    //创建房间请求
    void CreateRoomRq(sock_fd clientfd, char*szbuf, int nlen);
    //加入房间回复
    void JoinRoomRq(sock_fd clientfd, char*szbuf, int nlen);
    //退出房间处理
    void LeaveRoomRq(sock_fd clientfd, char*szbuf, int nlen);
    //处理音频帧
    void AudioFrameRq(sock_fd clientfd, char*szbuf, int nlen);
    //处理视频帧
    void VideoFrameRq(sock_fd clientfd, char *szbuf, int nlen);
    //音频注册
    void AudioRegister(sock_fd clientfd, char *szbuf, int nlen);
    //视频注册
    void VideoRegister(sock_fd clientfd, char *szbuf, int nlen);

    /*******************************************/
private:
    TcpKernel* m_pKernel;
    CMysql * m_sql;
    Block_Epoll_Net * m_tcp;
    //绑定id和用户信息
    MyMap<int,UserInfo*> m_mapIDToUserInfo;
    //保存用户id和其对应的房间号
    MyMap<int,list<int>> m_mapIDToRoomid;
};

#endif // CLOGIC_H
