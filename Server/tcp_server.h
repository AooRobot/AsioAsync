#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include "../Share/tcp_common.hpp"

class tcp_server : public tcp_comm
{
public:
	typedef tcp_session::ios_type  ios_type;
	typedef boost::asio::ip::tcp::acceptor  acceptor_type;

	typedef boost::asio::ip::tcp   tcp_type;
	typedef tcp_session::queue_type  queue_type;

	typedef boost::shared_ptr<io_service_pool>  io_service_pool_type;
public:
	tcp_server(unsigned short port, net_comm* net, int n = 4)
		:tcp_comm(net,n)
		,m_ios_pool(boost::make_shared<io_service_pool>(n))
		,m_queue(net->recv_queue())
		,m_acceptor(m_ios_pool->get(),tcp_type::endpoint(tcp_type::v4(),port))
	{
		start_accept();
	}

public:
	void start()
	{
		m_ios_pool->start();
	}

	void run()
	{
		m_ios_pool->run();
	}


protected:
	void start_accept()
	{
		tcp_session_ptr session = boost::make_shared<tcp_session>(m_ios_pool->get(), m_queue);

		m_acceptor.async_accept(session->socket(),
			boost::bind(&tcp_server::acceptor_handle, 
				this, 
				boost::asio::placeholders::error, 
				session)
		);
	}


	void acceptor_handle(const boost::system::error_code& error,tcp_session_ptr session)
	{
		start_accept();

		if (error)
		{
			session->close();
			return;
		}

		session->start();
		//session->io_service().dispatch(bind(&tcp_session::start, session));  
	}
private:
	io_service_pool_type  m_ios_pool;
	queue_type&  m_queue;    //连接请求队列
	acceptor_type  m_acceptor;
};



#endif // _TCP_SERVER_H
