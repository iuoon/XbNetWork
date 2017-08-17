//
//  XbClient.hpp
//  XbNetWork
//
//  Created by ÎâÓÀÕı on 2017/7/29.
//  Copyright ? 2017Äê ÎâÓÀÕı. All rights reserved.
//

#ifndef XbClient_hpp
#define XbClient_hpp

#include <stdio.h>
#include "XbCustom.h"

class XbClient {
    
public:
    socket_ptr sock;
    
    string userId;
    string userName;
    float health;
    float blueNum;
    
	string d_x;
	string d_y;
	string d_z;
    
	string p_x;
	string p_y;
	string p_z;
    
    float speed;
    
    string action;
    
};

#endif /* XbClient_hpp */
