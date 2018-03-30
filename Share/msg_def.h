#ifndef _MSG_DEF_H
#define _MSG_DEF_H

#include <boost/cstdint.hpp>

#define MAX_BODY_SIZE 1024

using boost::uint32_t;
typedef uint32_t uint32_t;  //boost

struct msg_head
{
	uint32_t type;   //�������
	uint32_t size;   //����
	uint32_t chksum;   //��ϢУ��� (��ֹ���ݱ��˴۸�)

};




#endif //_MSG_DEF_H
