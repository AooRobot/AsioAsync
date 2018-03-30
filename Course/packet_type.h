#ifndef _PACKET_TYPE_H
#define _PACKET_TYPE_H

typedef enum empacket_type
{
	PACKET_TEST = 0x800,   //测试包
	PACKET_LOGIN,          //登陆操作
	PACKET_ACK_LOGIN,      //登陆回复包
	PACKET_INFO,           //登陆时用到的信息
	PACKET_HEART,          //心跳包
}packet_type;







#endif // _PACKET_TYPE_H
