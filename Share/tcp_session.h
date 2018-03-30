#ifndef _TCP_SESSION_H
#define _TCP_SESSION_H

#include "tcp_message.h"
#include "job_queue.hpp"
#include "safe_atom.hpp"

#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/pool/object_pool.hpp>       //内存池



class tcp_session : public boost::enable_shared_from_this<tcp_session>
{
public:
	typedef boost::asio::ip::tcp::socket   socket_type;
	typedef boost::asio::io_service        ios_type;

	typedef ios_type::strand   strand_type;

	typedef job_queue<tcp_message*>   queue_type;

	typedef boost::object_pool<tcp_message>  object_pool_type;
public:
	tcp_session(ios_type& ios,queue_type& q);

	socket_type& socket();

	ios_type& io_service();

	void start();
	void close();

	bool is_connected()
	{
		bool b = false;
		m_bConnected.getValue(b);
		return b;
	}

	void set_connected(bool b)
	{
		m_bConnected.setValue(b);
	}

	void write(tcp_response_ptr resp);

private:
	tcp_message* create_request();

	void read(tcp_message* req);
	
private:
	socket_type        m_socket;
	queue_type&        m_queue;
	safe_atom<bool>    m_bConnected;

	strand_type        m_strand;

	static object_pool_type m_msg_pool;   //内存池  分配内存成功,session也能分配成功

protected:
	void handle_read_head(const boost::system::error_code& error, size_t bytes_transferred, tcp_message* req);
	void handle_read_msg(const boost::system::error_code& error, size_t bytes_transferred, tcp_message* req);
	void handle_write_head(const boost::system::error_code& error, size_t bytes_transferred, tcp_message* resp);
	void handle_write_msg(const boost::system::error_code& error, size_t bytes_transferred, tcp_message* resp);
};



#endif // _TCP_SESSION_H
