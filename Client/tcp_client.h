#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H

#include "../Share/tcp_common.hpp"


class tcp_client : public tcp_comm
{
public:
	tcp_client(boost::asio::ip::tcp::endpoint ep, net_comm* net, int n = 4);

protected:
private:
	void start_connect();

	void handle_connect(const boost::system::error_code& error, tcp_session_ptr session);

private:
	boost::asio::ip::tcp::endpoint  m_endpoint;
	tcp_session_ptr  m_session;
};






#endif // _TCPCLIENT_H
