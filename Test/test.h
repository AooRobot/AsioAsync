#ifndef _TEST_H
#define _TEST_H

#include "../Course/net_common.h"
#include <boost/shared_ptr.hpp>


class processor
{
public:
	processor(boost::shared_ptr<net_comm> net)
		:m_net(net)
	{

	}
public:
	virtual void proc_connected(tcp_session_ptr& session)
	{

	}

	virtual void proc_disconnected(tcp_session_ptr& session)
	{

	}
protected:
	boost::shared_ptr<net_comm> m_net;   //聚合关系
};


class server_processor : public processor
{
public:
	virtual void proc_connected(tcp_session_ptr& session)
	{
		std::cout << "有客户连接成功\n";
	}

	virtual void proc_disconnected(tcp_session_ptr& session)
	{
		std::cout << "客户端断开连接\n";
	}
	

protected:
private:
};

class client_processor : public processor
{
public:
	virtual void proc_connected(tcp_session_ptr& session)
	{
		std::cout << "连接服务器成功\n";
	}

	virtual void proc_disconnected(tcp_session_ptr& session)
	{
		std::cout << "断开服务器连接\n";
	}
};

#endif // _TEST_H
