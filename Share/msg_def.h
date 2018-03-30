#ifndef _MSG_DEF_H
#define _MSG_DEF_H

#include <boost/cstdint.hpp>

#define MAX_BODY_SIZE 1024

using boost::uint32_t;
typedef uint32_t uint32_t;  //boost

struct msg_head
{
	uint32_t type;   //封包类型
	uint32_t size;   //包长
	uint32_t chksum;   //消息校验和 (防止数据被人篡改)

};




#endif //_MSG_DEF_H
