#include "tcp_session.h"

tcp_session::object_pool_type tcp_session::m_msg_pool;

tcp_session::tcp_session(ios_type& ios, queue_type& q)
	:m_socket(ios)
	,m_strand(ios)
	,m_queue(q)
	,m_bConnected(false)
{

}

tcp_session::socket_type& tcp_session::socket()
{
	return m_socket;
}

tcp_session::ios_type& tcp_session::io_service()
{
	return m_socket.get_io_service();
}

void tcp_session::start()
{
	tcp_message* req = create_request();

	read(req);
}

void tcp_session::close()
{
	boost::system::error_code ignored_ec;

	m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

	m_socket.close(ignored_ec);
}


tcp_message* tcp_session::create_request()
{
	return m_msg_pool.construct(
		shared_from_this(),
		boost::bind(&object_pool_type::destroy, boost::ref(m_msg_pool), _1)
	);
}

void tcp_session::read(tcp_message* req)
{
	async_read(m_socket, boost::asio::buffer(req->head_data(), req->head_data().size())
		, m_strand.wrap(
			bind(&tcp_session::handle_read_head, 
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred,
				req))
	);
}


void tcp_session::handle_read_head(const boost::system::error_code& error, size_t bytes_transferred, tcp_message* req)
{
	if (error || !req->check_head())
	{
		this->close();
		req->destroy();
		return;
	}

	async_read(m_socket,
		boost::asio::buffer(req->msg_data(), req->get_head()->size)
		, m_strand.wrap(
			bind(&tcp_session::handle_read_msg,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred,
				req)
		)
	);
}

void tcp_session::handle_read_msg(const boost::system::error_code& error, size_t bytes_transferred, tcp_message* req)
{
	if (error || !req->check_msg_crc())
	{
		this->close();
		req->destroy();
		return;
	}

	m_queue.push(req);

	this->start();
}

void tcp_session::write(tcp_response_ptr resp)
{
	async_write(m_socket,
				boost::asio::buffer(resp->head_data().data(),resp->head_data().size()),
				m_strand.wrap(
					bind(&tcp_session::handle_write_head,
						shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred,
						resp)
			)
		);
}

void tcp_session::handle_write_head(const boost::system::error_code& error, size_t bytes_transferred, tcp_message* resp)
{
	if (error || resp->head_data().size() != bytes_transferred)
	{
		this->close();
		resp->destroy();
		return;
	}

	async_write(m_socket,
		boost::asio::buffer(resp->msg_data().data(), resp->get_head()->size),
		m_strand.wrap(
			bind(&tcp_session::handle_write_msg,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred,
				resp)
		)
	);
}

void tcp_session::handle_write_msg(const boost::system::error_code& error, size_t bytes_transferred, tcp_message* resp)
{
	if (error || resp->get_head()->size != bytes_transferred)
	{
		this->close();
		resp->destroy();
		return;
	}

	resp->destroy();  //发送成功后销毁
}