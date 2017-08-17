//
//  MessageHandler.cpp
//  XbNetWork
//
//  Created by 吴永正 on 2017/7/28.
//  Copyright ? 2017年 吴永正. All rights reserved.
//

#include "MessageHandler.hpp"
#include "UserManager.h"
#include "XbCmd.h"
#include "base64.h"

using namespace boost::property_tree;

MessageHandler::MessageHandler(){

}

MessageHandler::~MessageHandler(){
    
}

void MessageHandler::handleMsg(socket_ptr sock){
    char data[1024];
    boost::system::error_code error;
    size_t len = sock->read_some(buffer(data), error);
    if (len > 0) {		
        string recvdata(data);
		std::string _blen = recvdata.substr(6, 3);
		std::string _body = recvdata.substr(0,9+ atoi(_blen.c_str()));
        std::cout<<"收到消息："<< _body << std::endl;
        sock->write_some(buffer(distribute(_body,sock)));
    }
	//远程客户端发起关闭连接
    if (error == error::eof) {		
        sock->close();
		std::cout << "收到消息：客户端关闭连接" << std::endl;
        return;
    }
}

string MessageHandler::distribute(string message,socket_ptr sock){
    string _headCode=message.substr(0,4);
    string _userId=message.substr(4,2);
	string _blength = message.substr(6, 3);
	int len = message.length();
    int co;co=atoi(_headCode.c_str());
    string returnMsg;
    switch (co) {
        case PING_CODE:{
            returnMsg= "200"+_headCode+ _userId + _blength;
            break;
        }
        case LOGIN_CODE:{
			string msg = message.substr(9, len);
            XbClient * currentUser=NULL;
            iter = clients.find(_userId);
            if(iter!=clients.end()){
               currentUser =iter->second;
            }
            if(currentUser ==NULL){
                XbClient * user=new XbClient();
				user->sock = sock;
				user->userId = _userId;
				//解密
				string decMsg = base64_decode(msg);
				stringstream stream;
				stream << decMsg;
				boost::property_tree::ptree root;				
				read_json<ptree>(stream, root);
				user->d_x = root.get<string>("dx");
				user->d_y = root.get<string>("dy");
				user->d_z = root.get<string>("dz");
				user->p_x = root.get<string>("px");
				user->p_y = root.get<string>("py");
				user->p_z = root.get<string>("pz");
                clients.insert(pair<string,XbClient*>(_userId,user)); 				
            }
			returnMsg = "200" + _headCode + _userId ;
			ptree jsonc,users;
			string mc = "";
			//向在线玩家发送玩家上线
			if (clients.size()>1)
			{
				for (iter = clients.begin(); iter != clients.end(); iter++)
				{
					string uid = iter->first;
					XbClient* p = iter->second;
					if (uid != _userId)
					{
						string backMsg = "200" + _headCode + _userId + _blength + msg;
						p->sock->write_some(buffer(backMsg));
						ptree player;
						player.put("userId", uid);
						player.put("dx", p->d_x);
						player.put("dy", p->d_y);
						player.put("dz", p->d_z);
						player.put("px", p->p_x);
						player.put("py", p->p_y);
						player.put("pz", p->p_z);
						users.push_back(make_pair("", player));						
					}
				}
				jsonc.push_back(std::make_pair("users", users));
				ostringstream os;
				write_json(os, jsonc,false);  //false 禁止换行				
				mc = os.str();
			}			

			//返回附近玩家列表信息			
			mc = base64_encode(mc.c_str(), mc.length());
			_blength = "";
			if (10 < mc.length() && mc.length() < 100)
			{
				_blength = "0";
			}
			if (mc.length() < 10)
			{
				_blength = "00";
			}
			returnMsg = returnMsg + _blength+ std::to_string(mc.length())+mc;
            break;
        }
		case PLAYER_MOVE_CODE: {
			string msg = message.substr(9, len);
			returnMsg= "200" + _headCode + _userId + _blength + msg;
			for (iter = clients.begin(); iter != clients.end(); iter++)
			{
				string uid = iter ->first;
				XbClient* p = iter ->second;
				if (uid!=_userId)
				{	
					if (p->sock->is_open())
					{   
						try {
							p->sock->write_some(buffer(returnMsg));
						}
						catch (std::exception& e) {
							printf("caught exception: %s", e.what());
							// 通知用户离线
							clients.erase(uid);
						}											
					}
				}				
			}
			break;
		}
		case GET_PLAYER_LIST_CODE: {
			string msg = message.substr(9, len);
			returnMsg = "200" + _headCode + _userId + _blength + msg;
			break;
		}
        default:
            returnMsg="is error msg,goodbye";
            break;
    }
    return returnMsg;
}
