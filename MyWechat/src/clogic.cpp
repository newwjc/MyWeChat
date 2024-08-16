#include "clogic.h"
#include "TCPKernel.h"
void CLogic::setNetPackMap()
{
    NetPackMap(_DEF_PACK_REGISTER_RQ)    = &CLogic::RegisterRq;
    NetPackMap(_DEF_PACK_LOGIN_RQ)       = &CLogic::LoginRq;
    NetPackMap(_DEF_PACK_CREATEROOM_RQ)       = &CLogic::CreateRoomRq;
    NetPackMap(_DEF_PACK_JOINROOM_RQ)       = &CLogic::JoinRoomRq;
    NetPackMap(_DEF_PACK_LEAVEROOM_RQ)       = &CLogic::LeaveRoomRq;
    NetPackMap(_DEF_PACK_AUDIO_FRAME)       = &CLogic::AudioFrameRq;
    NetPackMap(_DEF_PACK_VIDEO_FRAME)       = &CLogic::VideoFrameRq;
    NetPackMap(_DEF_PACK_AUDIO_REGISTER)       = &CLogic::AudioRegister;
    NetPackMap(_DEF_PACK_VIDEO_REGISTER)       = &CLogic::VideoRegister;
}

#define _DEF_COUT_FUNC_    cout << "clientfd:"<< clientfd <<"  "<< __func__ << endl;

//注册请求
void CLogic::RegisterRq(sock_fd clientfd,char* szbuf,int nlen)
{
    //cout << "clientfd:"<< clientfd << __func__ << endl;
    _DEF_COUT_FUNC_;
    STRU_REGISTER_RQ * rq = (STRU_REGISTER_RQ*)szbuf;
    cout<<"tel"<<rq->tel<<"name"<<rq->name<<"pass"<<rq->password<<endl;
    char sqlStr[1024] = {0};
    sprintf(sqlStr,"select tel from t_user where tel = '%s';",rq->tel);
    list<string> resList;
    if(!m_sql->SelectMysql(sqlStr,1,resList)){
        printf("RegisterRq selectmysql error :%s \n",sqlStr);
        return ;
    }
    STRU_REGISTER_RS rs;
    if(resList.size()>0){
        rs.result = tel_is_exist;
    }else{
        memset(sqlStr,0,sizeof(sqlStr));
        resList.clear();
        sprintf(sqlStr,"select name from t_user where name = '%s';",rq->name);
        if(!m_sql->SelectMysql(sqlStr,1,resList)){
            printf("RegisterRq selectmysql error :%s \n",sqlStr);
            return ;
        }
        if(resList.size()>0){
            rs.result = name_is_exist;
        }else{
            rs.result = register_success;
            sprintf(sqlStr,"insert into t_user(tel,password,name,icon,feeling) values ('%s','%s','%s',%d,'%s');",rq->tel,rq->password,rq->name,1,"你说了什么");
            if(!m_sql->UpdataMysql(sqlStr)){
                printf("RegisterRq UpdataMysql error %s \n",sqlStr);
            }
        }
    }
    m_tcp->SendData(clientfd,(char*)&rs,sizeof(rs));
}

//登录请求
void CLogic::LoginRq(sock_fd clientfd ,char* szbuf,int nlen)
{
//    cout << "clientfd:"<< clientfd << __func__ << endl;
    _DEF_COUT_FUNC_ ;
    STRU_LOGIN_RQ * rq = (STRU_LOGIN_RQ*)szbuf;
    char sqlbuf[1024];
    memset(sqlbuf,0,sizeof(sqlbuf));
    list<string> lstRes;
    sprintf(sqlbuf,"select password,id,name from t_user where tel = '%s';",rq->tel);
    if(!m_sql->SelectMysql(sqlbuf,3,lstRes)){
        printf("LoginRq select error %s\n",sqlbuf);
        return ;
    }
    STRU_LOGIN_RS rs;
    if(lstRes.size()==0){
        rs.result = user_not_exist;
    }else{
        if(strcmp(rq->password,lstRes.front().c_str())!=0){
            rs.result=password_error;
        }else{
            lstRes.pop_front();
            //密码，帐号一直，保存id和sock
            int id = atoi(lstRes.front().c_str());
            lstRes.pop_front();
            UserInfo* pinfo = new UserInfo;
            pinfo->m_id=id;
            pinfo->m_roomid=0;
            pinfo->m_socket=clientfd;
            strcpy(pinfo->m_userName,lstRes.front().c_str());
            lstRes.pop_front();
            if(m_mapIDToUserInfo.IsExist(pinfo->m_id)){
                //强制下线todo
            }
            m_mapIDToUserInfo.insert(pinfo->m_id,pinfo);
            rs.userid=id;
            rs.result=login_success;
            strcpy(rs.m_name,pinfo->m_userName);
        }
    }
    SendData(clientfd,(char*)&rs,sizeof(rs));

//    STRU_LOGIN_RS rs;
//    rs.m_lResult = password_error;
    //    SendData( clientfd , (char*)&rs , sizeof rs );
}
//创建房间请求
void CLogic::CreateRoomRq(sock_fd clientfd, char *szbuf, int nlen)
{
    _DEF_COUT_FUNC_ ;
    STRU_CREATEROOM_RQ * rq = (STRU_CREATEROOM_RQ*)szbuf;
    int roomid =0;
    do{
        roomid = rand()%99999999 +1;
    }while(m_mapIDToRoomid.IsExist(roomid));
    list<int> lst;
    lst.push_back(rq->m_nUserId);
    m_mapIDToRoomid.insert(roomid,lst);
    STRU_CREATEROOM_RS rs;
    rs.m_RoomID=roomid;
    rs.m_lResult=1;
    SendData(clientfd,(char*)&rs,sizeof(rs));
}
//加入房间请求
void CLogic::JoinRoomRq(sock_fd clientfd, char *szbuf, int nlen)
{
    _DEF_COUT_FUNC_ ;
    STRU_JOINROOM_RQ * rq=(STRU_JOINROOM_RQ*)szbuf;
    STRU_JOINROOM_RS rs;
    //查看房间是否存在
    if(!m_mapIDToRoomid.IsExist(rq->m_RoomID)){
        //不存在，返回失败
        rs.m_lResult=0;
        SendData(clientfd,(char*)&rs,sizeof(rs));
        return ;
    }
    //存在，返回成功
    rs.m_lResult =1;
    rs.m_RoomID=rq->m_RoomID;
    SendData(clientfd,(char*)&rs,sizeof(rs));

    if(!m_mapIDToUserInfo.IsExist(rq->m_nUserId))return ;
    UserInfo * joiner = m_mapIDToUserInfo.find(rq->m_nUserId);

    STRU_ROOM_MEMBER_RQ joinrq;
    joinrq.m_nUserId=rq->m_nUserId;
    strcpy(joinrq.szUser,joiner->m_userName);
    //给自己用于更新的信息
    SendData(clientfd,(char*)&joinrq,sizeof(joinrq));
    //根据房间号拿到房间成员列表
    list<int> lstRoomMem = m_mapIDToRoomid.find(rq->m_RoomID);
    for(auto ite = lstRoomMem.begin();ite!=lstRoomMem.end();++ite){
        int Memid = *ite;
        if(!m_mapIDToUserInfo.IsExist(Memid))continue;
        UserInfo* memInfo = m_mapIDToUserInfo.find(Memid);
        STRU_ROOM_MEMBER_RQ memrq;
        memrq.m_nUserId=memInfo->m_id;
        strcpy(memrq.szUser,memInfo->m_userName);
        SendData(memInfo->m_socket,(char*)&joinrq,sizeof(joinrq));
        SendData(clientfd,(char*)&memrq,sizeof(memrq));
    }
    //加入人，添加到房间列表
    lstRoomMem.push_back(rq->m_nUserId);
    m_mapIDToRoomid.insert(rq->m_RoomID,lstRoomMem);

}
//退出房间处理
void CLogic::LeaveRoomRq(sock_fd clientfd, char *szbuf, int nlen)
{
    _DEF_COUT_FUNC_ ;
    //拆包
    STRU_LEAVEROOM_RQ *rq = (STRU_LEAVEROOM_RQ*)szbuf;
    //看房间是否存在
    if(!m_mapIDToRoomid.IsExist(rq->m_Roomid))return ;
    //如果房间存在 ，可以获得用户列表
    list<int> lst = m_mapIDToRoomid.find(rq->m_Roomid);
    //遍历每个用户 ，用户是否在线，在线转发从房间移除了
    for(auto ite = lst.begin();ite!=lst.end();){
        int userid = *ite;
        //是不是自己 ，如果是自己 从列表中去除
        if(userid == rq->m_nUserId){
            ite = lst.erase(ite);
        }else{
            if(m_mapIDToUserInfo.IsExist(userid)){
                UserInfo * info = m_mapIDToUserInfo.find(userid);
                SendData(info->m_socket,szbuf,nlen);
            }
            ++ite;
        }
    }
    //列表是否节点数为0 -> map项去掉
    if(lst.size() == 0){
        m_mapIDToRoomid.erase(rq->m_Roomid);
        return ;
    }
    //更新房间成员列表
    m_mapIDToRoomid.insert(rq->m_Roomid,lst);
}
//处理音频帧
void CLogic::AudioFrameRq(sock_fd clientfd, char *szbuf, int nlen)
{
    //拆包
    char * tmp = szbuf;
    //跳过type
    tmp += sizeof(int);
    //读取用户id
    int uid = *(int *)tmp;
    //跳过userid
    tmp+=sizeof(int);
    int roomid = *(int *)tmp;
    //看房间是否存在
    if(!m_mapIDToRoomid.IsExist(roomid))return ;
    list<int> lst = m_mapIDToRoomid.find(roomid);
    //获取成员列表
    for(auto ite = lst.begin();ite!=lst.end();++ite){
        //看是否在线 转发
        int userid = *ite;
        //屏蔽自己不转发
        if(uid == userid) continue;
        if(!m_mapIDToUserInfo.IsExist(userid)) continue;
        UserInfo * userinfo = m_mapIDToUserInfo.find(userid);
        //原数据转发
        //SendData(userinfo->m_socket,szbuf,nlen);
        SendData(userinfo->m_audiofd,szbuf,nlen);
    }

}
//处理视频帧
void CLogic::VideoFrameRq(sock_fd clientfd, char *szbuf, int nlen)
{
    //拆包
    char * tmp = szbuf;
    //跳过type
    tmp += sizeof(int);
    //读取用户id
    int uid = *(int *)tmp;
    //跳过userid
    tmp+=sizeof(int);
    int roomid = *(int *)tmp;
    //看房间是否存在
    if(!m_mapIDToRoomid.IsExist(roomid))return ;
    list<int> lst = m_mapIDToRoomid.find(roomid);
    //获取成员列表
    for(auto ite = lst.begin();ite!=lst.end();++ite){
        //看是否在线 转发
        int userid = *ite;
        //屏蔽自己不转发
        if(uid == userid) continue;
        if(!m_mapIDToUserInfo.IsExist(userid)) continue;
        UserInfo * userinfo = m_mapIDToUserInfo.find(userid);
        //原数据转发
        //SendData(userinfo->m_socket,szbuf,nlen);
        SendData(userinfo->m_videofd,szbuf,nlen);
    }

}
//音频注册
void CLogic::AudioRegister(sock_fd clientfd, char *szbuf, int nlen)
{
    //拆包
    STRU_AUDIO_REGISTER * rq = (STRU_AUDIO_REGISTER *)szbuf;
    int userid = rq->m_nUserId;
    //根据userid找到节点，更新fd
    if(m_mapIDToUserInfo.IsExist(userid)){
        UserInfo * info = m_mapIDToUserInfo.find(userid);
        info->m_audiofd = clientfd;
    }
}

//视频注册
void CLogic::VideoRegister(sock_fd clientfd, char *szbuf, int nlen)
{
    //拆包
    STRU_VIDEO_REGISTER * rq = (STRU_VIDEO_REGISTER *)szbuf;
    int userid = rq->m_nUserId;
    //根据userid找到节点，更新fd
    if(m_mapIDToUserInfo.IsExist(userid)){
        UserInfo * info = m_mapIDToUserInfo.find(userid);
        info->m_videofd = clientfd;
    }
}

















