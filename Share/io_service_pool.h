#ifndef  _IO_SERVICE_POOL_H
#define  _IO_SERVICE_POOL_H

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/thread.hpp>
#include <boost/functional/factory.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

class io_service_pool :public boost::noncopyable   //可以含有多个io_service  '大酒店'
{
public:
	typedef boost::asio::io_service  ios_type;			// io操作对象  异步服务工具
	typedef boost::asio::io_service::work  work_type;   //确保io_service正常运行
	typedef boost::ptr_vector<ios_type>  io_services_type;
	typedef boost::ptr_vector<work_type>  works_type;

public:
	explicit io_service_pool(int n = 4);    //explicit 只能通过这种方式构造类对象,没有隐藏的构造方法

	ios_type& get();  //轮询方法,从外部获取io_service对象,用于组成acceptor对象

	void start();    //启动线程池
	void stop();	 //停止线程池
	void run();		 //线程池运行

protected:
private:
	io_services_type  m_io_services;
	works_type  m_works;   //保证io_services都正常工作

	boost::thread_group  m_threads;    //线程组
	std::size_t  m_next_io_services;   //用于轮询使用

	void init(int n);    //创建一系列io_services对象并加入到m_io_services
};

#endif // _IO_SERVICE_POOL_H
