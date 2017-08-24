//
//  main.cpp
//  XbNetWork
//
//  Created by 吴永正 on 2017/7/24.
//  Copyright ? 2017年 吴永正. All rights reserved.
//

#include "XbCustom.h"
#include "MessageHandler.hpp"

using namespace boost::property_tree;

void client_session(socket_ptr sock);

void checkheart();

MessageHandler * messageHandler;


int main(int argc, char* argv[]) {

	std::cout << "服务监听已启动：..." << std::endl;
	srand((int)time(NULL));
	int a = 10+ rand() % 99;
	std::cout << a << std::endl;

	io_service service;
	ip::tcp::endpoint ep(ip::tcp::v4(), 9001);//
	ip::tcp::acceptor acc(service, ep);
	messageHandler = new MessageHandler();

	while (true) {
		socket_ptr sock(new ip::tcp::socket(service));
		acc.accept(*sock);
		boost::thread(boost::bind(client_session, sock));
	}
}

void client_session(socket_ptr sock) {
	while (sock->is_open())
	{
		messageHandler->handleMsg(sock);		
	}			
}

void checkheart() {

}


