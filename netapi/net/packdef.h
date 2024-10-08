#pragma once

#include<memory.h>

#pragma once

//边界值
#define _DEF_SIZE           45
#define _DEF_BUFFERSIZE     1000
#define _DEF_PORT           8000
#define _DEF_SERVERIP       "192.168.32.133"
#define _DEF_LISTEN         128
#define _DEF_EPOLLSIZE      4096
#define _DEF_IPSIZE         16
#define _DEF_COUNT          10
#define _DEF_TIMEOUT        10
#define _DEF_SQLIEN         400
#define TRUE                true
#define FALSE               false



/*-------------数据库信息-----------------*/
#define _DEF_DB_NAME    "my_wechat"
#define _DEF_DB_IP      "localhost"
#define _DEF_DB_USER    "root"
#define _DEF_DB_PWD     "123456"
/*--------------------------------------*/
#define MAX_PATH           (260)
#define _DEF_BUFFER         (4096)
#define _DEF_CONTENT_SIZE	(1024)
#define _MAX_SIZE           (40)

//自定义协议   先写协议头 再写协议结构
//登录 注册 获取好友信息 添加好友 聊天 发文件 下线请求
#define _DEF_PACK_BASE	(10000)
#define _DEF_PACK_COUNT (100)

//注册
#define _DEF_PACK_REGISTER_RQ	(_DEF_PACK_BASE + 0 )
#define _DEF_PACK_REGISTER_RS	(_DEF_PACK_BASE + 1 )
//登录
#define _DEF_PACK_LOGIN_RQ	(_DEF_PACK_BASE + 2 )
#define _DEF_PACK_LOGIN_RS	(_DEF_PACK_BASE + 3 )
//创建房间
#define _DEF_PACK_CREATEROOM_RQ	(_DEF_PACK_BASE + 4 )
#define _DEF_PACK_CREATEROOM_RS	(_DEF_PACK_BASE + 5 )
//加入房间
#define _DEF_PACK_JOINROOM_RQ	(_DEF_PACK_BASE + 6 )
#define _DEF_PACK_JOINROOM_RS	(_DEF_PACK_BASE + 7 )
//房间列表请求
#define _DEF_PACK_ROOM_MEMBER	(_DEF_PACK_BASE + 8 )
//音频数据
#define _DEF_PACK_AUDIO_FRAME	(_DEF_PACK_BASE + 9 )
//视频数据
#define _DEF_PACK_VIDEO_FRAME	(_DEF_PACK_BASE + 10 )
//退出房间请求
#define _DEF_PACK_LEAVEROOM_RQ	(_DEF_PACK_BASE + 11 )
//退出房间回复
#define _DEF_PACK_LEAVEROOM_RS	(_DEF_PACK_BASE + 12 )
//音频注册
#define _DEF_PACK_AUDIO_REGISTER    (_DEF_PACK_BASE + 13 )
//视频注册
#define _DEF_PACK_VIDEO_REGISTER	(_DEF_PACK_BASE + 14 )



//返回的结果
//注册请求的结果
#define tel_is_exist		(0)
#define name_is_exist		(1)
#define register_success	(2)
//登录请求的结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)


typedef int PackType;

//协议结构
//注册
typedef struct STRU_REGISTER_RQ
{
    STRU_REGISTER_RQ():type(_DEF_PACK_REGISTER_RQ)
    {
        memset( tel  , 0, sizeof(tel));
        memset( name  , 0, sizeof(name));
        memset( password , 0, sizeof(password) );
        //memset( feeling , 0, sizeof(feeling) );
    }
    //需要手机号码 , 密码, 昵称
    PackType type;
    char tel[_MAX_SIZE];
    char name[_MAX_SIZE];
    char password[_MAX_SIZE];
    //char feeling[_MAX_SIZE];

}STRU_REGISTER_RQ;

typedef struct STRU_REGISTER_RS
{
    //回复结果
    STRU_REGISTER_RS(): type(_DEF_PACK_REGISTER_RS) , result(register_success)
    {
    }
    PackType type;
    int result;

}STRU_REGISTER_RS;

//登录
typedef struct STRU_LOGIN_RQ
{
    //登录需要: 手机号 密码
    STRU_LOGIN_RQ():type(_DEF_PACK_LOGIN_RQ)
    {
        memset( tel , 0, sizeof(tel) );
        memset( password , 0, sizeof(password) );
    }
    PackType type;
    char tel[_MAX_SIZE];
    char password[_MAX_SIZE];

}STRU_LOGIN_RQ;
//登录回复
typedef struct STRU_LOGIN_RS
{
    // 需要 结果 , 用户的id
    STRU_LOGIN_RS(): type(_DEF_PACK_LOGIN_RS) , result(login_success),userid(0)
    {
        memset(m_name,0,_MAX_SIZE);
    }
    PackType type;
    int result;
    int userid;
    char m_name[_MAX_SIZE];

}STRU_LOGIN_RS;
//用户信息
typedef struct UserInfo{
    UserInfo(){
        m_socket = 0;
        m_id = 0;
        m_roomid = 0;
        memset(m_userName,0,_MAX_SIZE);
    }
    PackType type;
    int m_socket;
    int m_id;
    int m_roomid;
    char m_userName[_MAX_SIZE];

    int m_videofd;
    int m_audiofd;
}UserInfo ;


//创建房间请求
typedef struct STRU_CREATEROOM_RQ{
    STRU_CREATEROOM_RQ(){
        type = _DEF_PACK_CREATEROOM_RQ;
        m_nUserId = 0;
    }
    PackType type;
    int m_nUserId;
}STRU_CREATEROOM_RQ;


//创建房间回复
typedef struct STRU_CREATEROOM_RS{
    STRU_CREATEROOM_RS(){
        type = _DEF_PACK_CREATEROOM_RS;
        m_RoomID = 0;
        m_lResult = 0;
    }
    PackType type;
    int m_RoomID;
    int m_lResult;
}STRU_CREATEROOM_RS;

//加入房间请求
typedef struct STRU_JOINROOM_RQ{
    STRU_JOINROOM_RQ(){
        type = _DEF_PACK_JOINROOM_RQ;
        m_RoomID = 0;
        m_nUserId = 0;
    }
    PackType type;
    int m_RoomID;
    int m_nUserId;
}STRU_JOINROOM_RQ;

//加入房间回复
typedef struct STRU_JOINROOM_RS{
    STRU_JOINROOM_RS(){
        type = _DEF_PACK_JOINROOM_RS;
        m_RoomID = 0;
        m_lResult = 0;
    }
    PackType type;
    int m_RoomID;
    int m_lResult;
}STRU_JOINROOM_RS;

//房间成员请求
typedef struct STRU_ROOM_MEMBER_RQ{
    STRU_ROOM_MEMBER_RQ(){
        type = _DEF_PACK_ROOM_MEMBER;
        m_nUserId = 0;
        memset(szUser,0,_MAX_SIZE);
    }
    PackType type;
    int m_nUserId;
    char szUser[_MAX_SIZE];
}STRU_ROOM_MEMBER_RQ;

//离开房间请求
typedef struct STRU_LEAVEROOM_RQ{
    STRU_LEAVEROOM_RQ(){
        type = _DEF_PACK_LEAVEROOM_RQ;
        m_nUserId = 0;
        m_Roomid = 0;
        memset(szUserName,0,_MAX_SIZE);
    }
    PackType type;
    int m_nUserId;
    int m_Roomid;
    char szUserName[_MAX_SIZE];
}STRU_LEAVEROOM_RQ;

//音频注册
typedef struct STRU_AUDIO_REGISTER{
    STRU_AUDIO_REGISTER(){
        type = _DEF_PACK_AUDIO_REGISTER;
        m_nUserId = 0;
    }
    PackType type;
    int m_nUserId;
}STRU_AUDIO_REGISTER;
//视频注册
typedef struct STRU_VIDEO_REGISTER{
    STRU_VIDEO_REGISTER(){
        type = _DEF_PACK_VIDEO_REGISTER;
        m_nUserId = 0;
    }
    PackType type;
    int m_nUserId;
}STRU_VIDEO_REGISTER;


///视频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int min;
/// int sec;
/// int msec;
/// QByteArray videoFrame;
///


///音频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int min;
/// int sec;
/// int msec;
/// QByteArray audioFrame;
///

///视频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int min;
/// int sec;
/// int msec;
/// QByteArray videoFrame;
///


