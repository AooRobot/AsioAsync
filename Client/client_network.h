#ifndef _CLIENT_NETWORK_H
#define _CLIENT_NETWORK_H


#include "../Course/net_common.h"
#include "tcp_client.h"


class client_network : public net_comm
{
	typedef boost::asio::ip::tcp     tcp_type;
public:
	client_network(tcp_type::endpoint ep, int poolnum, int sec_check = 3);

	virtual ~client_network();

public:
	void start();
	void run();

protected:
	virtual bool proc_connect(tcp_session_ptr& session);
	virtual bool proc_disconnect(tcp_session_ptr& session);
	virtual bool proc_packet(tcp_session_ptr& session, serial_packet* packet);
	virtual void proc_register_packet();
	virtual void check_session_status();


private:
	void send_heart_packet();
private:
	boost::shared_ptr<tcp_client> m_tcp_client;
};



#endif //_CLIENT_NETWORK_H
