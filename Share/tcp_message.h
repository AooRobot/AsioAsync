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

	bool check_head();  //�����Ϣͷ�Ƿ���ȷ

	bool check_msg_crc();   //����CRC
	void set_msg_crc();     //���CRC

	void destroy();    //������Ϣ

	tcp_session_ptr get_session()
	{
		return m_session;
	}
	
	msg_head* get_head();
private:
	head_data_type m_head;   //��Ϣͷ
	body_data_type m_msg;    //��Ϣ
	destroy_type   m_destroy;    //�����õ��Զ��庯��
	tcp_session_ptr m_session;   //˭������Ϣ
};

typedef tcp_message tcp_request;    //tcp������Ϣ
typedef tcp_message tcp_response;   //tcp��Ӧ��Ϣ

typedef tcp_request* tcp_request_ptr;	  //tcp������Ϣָ��
typedef tcp_response* tcp_response_ptr;   //tcp��Ӧ��Ϣָ��


#endif // _TCP_MESSAGE_H
