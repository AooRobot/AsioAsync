#ifndef _NET_COMMON_H
#define _NET_COMMON_H

#include "../Share/job_queue.hpp"
#include "../Share/tcp_message.h"
#include "../Share/worker.hpp"
#include "../Share/scheduler.hpp"
#include "../Share/safe_vector.hpp"
#include "../Share/tcp_session.h"

#include "serial_packet.h"
#include "packet_factory.h"

/************************************************************************/
/* 作者: 曹计彦博
/* 时间: 2018/01/31 10:14
/* 说明:
/* 实现网络事件的回调                                                                
/************************************************************************/

class net_comm
{
	friend class tcp_comm;     //tcp_comm去访问net_comm tcp_comm
public:
	typedef job_queue<tcp_request_ptr>   req_queue_type;    //接收消息队列
	typedef job_queue<tcp_response_ptr>  resp_queue_type;   //发送消息队列

	typedef worker<req_queue_type>   req_worker_type;    //处理已经收到的消息
	typedef worker<resp_queue_type>  resp_worker_type;   //处理待发送的消息

	typedef scheduler  scheduler_type;     //定时执行一些功能

	typedef safe_vector<tcp_session_ptr>  vector_session_type;    //管理连接会话
public:
	net_comm(int sec_check = 1);

	virtual ~net_comm()
	{

	}
public:
	void start();   //启动工作线程

	req_queue_type& recv_queue();
	resp_queue_type& send_queue();



	//-----------接口-------------
	virtual bool proc_connect(tcp_session_ptr& session) = 0;
	virtual bool proc_disconnect(tcp_session_ptr& session) = 0;
	virtual bool proc_packet(tcp_session_ptr& session, serial_packet* packet) = 0;  
	
	virtual void proc_register_packet() = 0;

	virtual void check_session_status() {};  //子类实现就用子类的   继承者可以不用去检查网络状态

protected:
	bool session_connect(tcp_session_ptr& session);
	bool session_disconnect(tcp_session_ptr& session);

protected:
	bool process_msg(tcp_request_ptr& req);
	bool send_msg(tcp_response_ptr& resp);

	bool send_packet(tcp_session_ptr& sp, serial_packet& packet);
	void generate_session_data(/*IN*/ tcp_response_ptr& resp, /*OUT*/ serial_packet& packet);

	bool add_session(const tcp_session_ptr& session);
	bool remove_session(const tcp_session_ptr& session);

	bool find_session(const tcp_session_ptr& session);

protected:
	vector_session_type  m_tcp_sessions;  //这个里面用来保存已经了解成功的会话

private:
	req_queue_type   m_recvq;     //request就是recv
	resp_queue_type  m_sendp;     //response就是send

	req_worker_type  m_req_worker;   //处理已接受消息的工作线程组
	resp_worker_type m_resp_worker;  //处理发送消息的工作线程组

	packet_factory   m_packet_facotry;    //包生成工厂

	scheduler_type   m_scheduler;    //定时执行任务
};





#endif //_NET_COMMON_H
