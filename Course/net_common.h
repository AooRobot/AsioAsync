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
/* ����: �ܼ��岩
/* ʱ��: 2018/01/31 10:14
/* ˵��:
/* ʵ�������¼��Ļص�                                                                
/************************************************************************/

class net_comm
{
	friend class tcp_comm;     //tcp_commȥ����net_comm tcp_comm
public:
	typedef job_queue<tcp_request_ptr>   req_queue_type;    //������Ϣ����
	typedef job_queue<tcp_response_ptr>  resp_queue_type;   //������Ϣ����

	typedef worker<req_queue_type>   req_worker_type;    //�����Ѿ��յ�����Ϣ
	typedef worker<resp_queue_type>  resp_worker_type;   //��������͵���Ϣ

	typedef scheduler  scheduler_type;     //��ʱִ��һЩ����

	typedef safe_vector<tcp_session_ptr>  vector_session_type;    //�������ӻỰ
public:
	net_comm(int sec_check = 1);

	virtual ~net_comm()
	{

	}
public:
	void start();   //���������߳�

	req_queue_type& recv_queue();
	resp_queue_type& send_queue();



	//-----------�ӿ�-------------
	virtual bool proc_connect(tcp_session_ptr& session) = 0;
	virtual bool proc_disconnect(tcp_session_ptr& session) = 0;
	virtual bool proc_packet(tcp_session_ptr& session, serial_packet* packet) = 0;  
	
	virtual void proc_register_packet() = 0;

	virtual void check_session_status() {};  //����ʵ�־��������   �̳��߿��Բ���ȥ�������״̬

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
	vector_session_type  m_tcp_sessions;  //����������������Ѿ��˽�ɹ��ĻỰ

private:
	req_queue_type   m_recvq;     //request����recv
	resp_queue_type  m_sendp;     //response����send

	req_worker_type  m_req_worker;   //�����ѽ�����Ϣ�Ĺ����߳���
	resp_worker_type m_resp_worker;  //��������Ϣ�Ĺ����߳���

	packet_factory   m_packet_facotry;    //�����ɹ���

	scheduler_type   m_scheduler;    //��ʱִ������
};





#endif //_NET_COMMON_H
