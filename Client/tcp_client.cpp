#include "tcp_client.h"


tcp_client::tcp_client(boost::asio::ip::tcp::endpoint ep, net_comm* net, int n)
	:tcp_comm(net,n)
	,m_endpoint(ep)
	,m_session(boost::factory<tcp_session_ptr>()(m_ios_pool.get(),net->recv_queue()))
{

}


void tcp_client::start_connect()
{
	if (m_session->is_connected())
	{
		return;
	}

	m_session->socket().async_connect(m_endpoint,
		boost::bind(&tcp_client::handle_connect
			, this
			, boost::asio::placeholders::error
			, m_session)
	);
}


void tcp_client::handle_connect(const boost::system::error_code& error, tcp_session_ptr session)
{
	if (error)
	{
		session->close();
		return;
	}

	session->start();
}