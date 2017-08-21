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
#include "XbUtil.h"

using namespace boost::property_tree;

MessageHandler::MessageHandler(){

}

MessageHandler::~MessageHandler(){
    
}

void MessageHandler::handleMsg(socket_ptr sock){
    char data[1024];
    boost::system::error_code err;
    size_t len = sock->read_some(buffer(data), err);
	
    if (len > 7) {		
        string recvdata(data);
		std::string _blen = recvdata.substr(4, 3);
		std::string _body = recvdata.substr(0,7+ atoi(_blen.c_str()));
   //     std::cout<<"收到消息："<< _body << std::endl;
        sock->write_some(buffer(distribute(_body,sock)));
	}
	else {
		//string recvdata1(data);
		//std::cout << "收到错误的消息:" <<std::endl;
	}
	if (len == 0 || err == error::eof)
	{
		sock->close();
		std::cout << "recive msg：the client closed!" << std::endl;
	}
	
}

string MessageHandler::distribute(string message,socket_ptr sock){
    string _headCode=message.substr(0,4);
	string _blength = message.substr(4, 3);
	int len = message.length();
    int co;co=atoi(_headCode.c_str());
    string returnMsg;
	string msg = message.substr(7, len);
	//解密
	string decMsg = base64_decode(msg);
	stringstream stream;
	stream << decMsg;
	boost::property_tree::ptree root;
	read_json<ptree>(stream, root);
	string _userId = root.get<string>("userId");

    switch (co) {
        case PING_CODE:{
            returnMsg= "200"+_headCode+ _userId + _blength;
            break;
        }
        case LOGIN_CODE:{
			
            XbClient * currentUser=NULL;
            iter = clients.find(_userId);
            if(iter!=clients.end()){
               currentUser =iter->second;
            }
            if(currentUser ==NULL){
                XbClient * user=new XbClient();
				user->sock = sock;
				user->userId = _userId;
				
				user->d_x = root.get<string>("dx");
				user->d_y = root.get<string>("dy");
				user->d_z = root.get<string>("dz");
				user->p_x = root.get<string>("px");
				user->p_y = root.get<string>("py");
				user->p_z = root.get<string>("pz");
                clients.insert(pair<string,XbClient*>(_userId,user)); 				
            }
			returnMsg = "200" + _headCode;
			ptree jsonc,users;
			string mc = "";
			//向在线玩家发送玩家上线
			if (clients.size()>1)
			{
				for (iter = clients.begin(); iter != clients.end();)
				{
					string uid = iter->first;
					XbClient* p = iter->second;
					if (uid != _userId)
					{
						string backMsg = "200" + _headCode + _blength + msg;
						boost::system::error_code error1;
						
						p->sock->write_some(buffer(backMsg),error1);
						
						if (error1 == error::eof) {
							// 通知用户离线
							clients.erase(iter++);					
						}
																		
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
					++iter;
				}
				jsonc.push_back(std::make_pair("users", users));
				jsonc.put("userId", _userId);
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
			returnMsg = returnMsg + _blength+ xb_tostring(mc.length())+mc;
			
            break;
        }
		case PLAYER_MOVE_CODE: {
			string msg = message.substr(7, len);
			returnMsg= "200" + _headCode  + _blength + msg;
			for (iter = clients.begin(); iter != clients.end(); )
			{
				string uid = iter ->first;
				XbClient* p = iter ->second;
				if (uid!=_userId)
				{	
					if (p->sock->is_open())
					{   
						boost::system::error_code error2;
						p->sock->write_some(buffer(returnMsg), error2);
						if (error2 == error::eof) {
							// 通知用户离线
							clients.erase(iter++);
						}						
					}
				}
				++iter;
			}
			break;
		}
		case GET_PLAYER_LIST_CODE: {
			string msg = message.substr(7, len);
			returnMsg = "200" + _headCode  + _blength + msg;
			break;
		}
        default:
            returnMsg="is error msg,goodbye";
            break;
    }
    return returnMsg;
}
