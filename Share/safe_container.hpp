#ifndef _SAFE_CONTAINER_H
#define _SAFE_CONTAINER_H

#include <algorithm>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/lock_types.hpp>

/************************************************************************/
/* ����: �ܼ��岩
/* ʱ��: 2018/01/29 10:20
/* ˵��: ���������Ϊ��ɾ���������캯��,�̳������������඼�����п�������
/*       ����                                                          
/************************************************************************/

class safe_container : public boost::noncopyable
{
public:
	typedef boost::shared_mutex rw_mutex;				//��������
	typedef boost::shared_lock<rw_mutex>	read_lock;
	typedef boost::unique_lock<rw_mutex>	write_lock;
protected:
	mutable rw_mutex m_mutex;  //����mutable ������const����
};

#endif // _SAFE_CONTAINER_H

