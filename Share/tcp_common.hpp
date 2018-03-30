#ifndef _TCP_COMMON_H
#define _TCP_COMMON_H

#include "io_service_pool.h"
#include "msg_def.h"
#include "scheduler.hpp"
#include "tcp_session.h"

#include <boost/functional/factory.hpp>

#include "../Course/net_common.h"

class tcp_comm
{
public:
	typedef io_service_pool::ios_type         ios_type;
	typedef boost::asio::ip::tcp::acceptor    acceptor_type;
	typedef boost::asio::ip::tcp              tcp_type;
	typedef tcp_session::queue_type           queue_type;
	typedef boost::function<bool(tcp_session_ptr &)>  function_type;

	typedef scheduler      scheduler_type;

public:
	tcp_comm(net_comm* net,int n = 4)
		:m_ios_pool(*boost::factory<io_service_pool*>()(n))
		,m_queue(net->recv_queue())
		,m_connect_func(boost::bind(&net_comm::session_connect,net,_1))
		,m_disconnect_func(boost::bind(&net_comm::session_disconnect,net,_1))
	{

	}
public:
	void start()
	{
		m_ios_pool.start();
	}

	void run()
	{
		m_ios_pool.run();
	}
protected:

	io_service_pool&  m_ios_pool;  //并发线程池
	function_type  m_connect_func;  //连接
	function_type  m_disconnect_func;  //断开连接
	queue_type&  m_queue;    //工作对列,即消息队列

};





#endif // _TCP_COMMON_H