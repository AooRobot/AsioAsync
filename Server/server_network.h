#ifndef _SERVER_NETWORK_H
#define _SERVER_NETWORK_H

#include "../Course/net_common.h"
#include "tcp_server.h"


class server_network : public net_comm
{
public:
	server_network(int port, int poolnum, int sec_check = 3);

	virtual ~server_network();

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
	boost::shared_ptr<tcp_server> m_tcp_server;
};






#endif // _SERVER_NETWORK_H
