#ifndef _TCP_MESSAGE_H
#define _TCP_MESSAGE_H

#include "msg_def.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/crc.hpp>
#include <boost/function.hpp>

class tcp_session;
typedef boost::shared_ptr<tcp_session> tcp_session_ptr;

class tcp_message : public boost::noncopyable
{
public:
	typedef char char_type;
	typedef boost::array<char_type, sizeof(msg_head)> head_data_type;
	typedef boost::array<char_type, MAX_BODY_SIZE>    body_data_type;

	typedef boost::function<void(tcp_message*)> destroy_type;
public:
	template<typename Func>
	tcp_message(const tcp_session_ptr& s,Func func)
		:m_session(s)
		,m_destroy(func)
	{

	}

	tcp_message(const tcp_session_ptr& s)
		:m_session(s)
	{

	}

public:
	head_data_type& head_data();
	body_data_type& msg_data();

	bool check_head();  //检查消息头是否正确

	bool check_msg_crc();   //设置CRC
	void set_msg_crc();     //检查CRC

	void destroy();    //销毁消息

	tcp_session_ptr get_session()
	{
		return m_session;
	}
	
	msg_head* get_head();
private:
	head_data_type m_head;   //消息头
	body_data_type m_msg;    //消息
	destroy_type   m_destroy;    //销毁用的自定义函数
	tcp_session_ptr m_session;   //谁发的消息
};

typedef tcp_message tcp_request;    //tcp请求消息
typedef tcp_message tcp_response;   //tcp响应消息

typedef tcp_request* tcp_request_ptr;	  //tcp请求消息指针
typedef tcp_response* tcp_response_ptr;   //tcp响应消息指针


#endif // _TCP_MESSAGE_H
