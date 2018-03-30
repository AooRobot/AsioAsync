#ifndef _SAFE_CONTAINER_H
#define _SAFE_CONTAINER_H

#include <algorithm>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/lock_types.hpp>

/************************************************************************/
/* 作者: 曹计彦博
/* 时间: 2018/01/29 10:20
/* 说明: 类的作用是为了删除拷贝构造函数,继承自这个类的子类都不会有拷贝构造
/*       能力                                                          
/************************************************************************/

class safe_container : public boost::noncopyable
{
public:
	typedef boost::shared_mutex rw_mutex;				//共享互斥体
	typedef boost::shared_lock<rw_mutex>	read_lock;
	typedef boost::unique_lock<rw_mutex>	write_lock;
protected:
	mutable rw_mutex m_mutex;  //加了mutable 可以用const修饰
};

#endif // _SAFE_CONTAINER_H

