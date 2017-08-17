//
//  XbCustom.h
//  XbNetWork
//
//  Created by ������ on 2017/7/29.
//  Copyright ? 2017�� ������. All rights reserved.
//

#ifndef XbCustom_h
#define XbCustom_h

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/json_parser.hpp>  
#include <boost/foreach.hpp> 
using namespace boost::asio;
using namespace std;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;



#endif /* XbCustom_h */
