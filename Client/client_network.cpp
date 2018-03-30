#include "client_network.h"

client_network::client_network(tcp_type::endpoint ep, int poolnum, int sec_check /* = 3 */)
	:net_comm(sec_check)
{
	//m_tcp_client = new tcp_client(ep, this, poolnum);
	//m_tcp_client = boost::factory<tcp_client*>()(ep, this, poolnum);
	m_tcp_client = boost::make_shared<tcp_client>(ep, this, poolnum);
}

client_network::~client_network()
{

}

bool client_network::proc_connect(tcp_session_ptr& session)
{
	std::cout << "连接成功\n";
	return true;
}

bool client_network::proc_disconnect(tcp_session_ptr& session)
{
	std::cout << "断开连接\n";
	return true;
}

bool client_network::proc_packet(tcp_session_ptr& session, serial_packet* packet)
{
	std::cout << "接收到封包\n";
	switch (packet->get_packet_type())
	{
	case PACKET_TEST:
		break;
	case PACKET_LOGIN:
		break;
	case PACKET_INFO:
		break;
	default:
		break;
	}
	return true;
}

void  client_network::proc_register_packet()
{

}

void client_network::check_session_status()
{

}