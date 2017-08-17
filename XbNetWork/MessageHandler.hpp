//
//  MessageHandler.hpp
//  XbNetWork
//
//  Created by ÎâÓÀÕı on 2017/7/28.
//  Copyright ? 2017Äê ÎâÓÀÕı. All rights reserved.
//

#ifndef MessageHandler_hpp
#define MessageHandler_hpp


#include <stdio.h>
#include "XbCustom.h"


class MessageHandler {
public:
    MessageHandler();
    ~MessageHandler();
    void handleMsg(socket_ptr sock);
    string distribute(string message, socket_ptr sock);
private:   
    
    
};

#endif /* MessageHandler_hpp */
