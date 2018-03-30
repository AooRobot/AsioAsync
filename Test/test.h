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
	boost::shared_ptr<net_comm> m_net;   //�ۺϹ�ϵ
};


class server_processor : public processor
{
public:
	virtual void proc_connected(tcp_session_ptr& session)
	{
		std::cout << "�пͻ����ӳɹ�\n";
	}

	virtual void proc_disconnected(tcp_session_ptr& session)
	{
		std::cout << "�ͻ��˶Ͽ�����\n";
	}
	

protected:
private:
};

class client_processor : public processor
{
public:
	virtual void proc_connected(tcp_session_ptr& session)
	{
		std::cout << "���ӷ������ɹ�\n";
	}

	virtual void proc_disconnected(tcp_session_ptr& session)
	{
		std::cout << "�Ͽ�����������\n";
	}
};

#endif // _TEST_H
