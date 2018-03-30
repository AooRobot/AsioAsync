#include "server_network.h"
#include "heart_packet.h"

server_network::server_network(int port, int poolnum, int sec_check)
	:net_comm(sec_check)
{
	m_tcp_server = boost::make_shared<tcp_server>(port, this, poolnum);
}

server_network::~server_network()
{

}

void server_network::start()   //非阻塞运行
{
	this->proc_register_packet();

	net_comm::start();

	m_tcp_server->start();
}

void server_network::run()    //阻塞运行
{
	this->proc_register_packet();

	net_comm::start();

	m_tcp_server->run();
}

bool server_network::proc_connect(tcp_session_ptr& session)
{
	std::cout << "连接成功\n";
	return true;
}

bool server_network::proc_disconnect(tcp_session_ptr& session)
{
	std::cout << "断开连接\n";
	return true;
}

bool server_network::proc_packet(tcp_session_ptr& session, serial_packet* packet)
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

void  server_network::proc_register_packet()
{

}

void  server_network::check_session_status()
{
	send_heart_packet();    //发送心跳包
}

//----------------------------
void server_network::send_heart_packet()
{
	heart_packet  packet;

	packet.m_hour = 9;
	packet.m_min = 9;
	packet.m_sec = 9;
	packet.m_milsec = 9;

	vector_session_type::iterator iter = m_tcp_sessions.begin();    //向所有连接成功的会话发送心跳包

	for (;iter != m_tcp_sessions.end();++iter)
	{
		send_packet(*iter, packet);
	}
}