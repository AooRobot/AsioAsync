#include "net_common.h"

net_comm::net_comm(int sec_check)
	:m_req_worker(m_recvq,boost::bind(&net_comm::process_msg,this,_1) )
	,m_resp_worker(m_sendp, boost::bind(&net_comm::send_msg,this,_1) )
{
	m_req_worker.set_workFunc(boost::bind(&net_comm::process_msg, this, _1));
	m_resp_worker.set_workFunc(boost::bind(&net_comm::send_msg, this, _1));

	m_scheduler.add(boost::bind(&net_comm::check_session_status, this), sec_check);
}

void net_comm::start()
{
	m_req_worker.start();
	m_resp_worker.start();
}


bool net_comm::session_connect(tcp_session_ptr& session)
{
	return proc_connect(session);
}

bool net_comm::session_disconnect(tcp_session_ptr& session)
{
	return proc_disconnect(session);
}

net_comm::req_queue_type& net_comm::recv_queue()
{
	return m_recvq;
}

net_comm::resp_queue_type& net_comm::send_queue()
{
	return m_sendp;
}

bool net_comm::process_msg(tcp_request_ptr& req)
{
	msg_head* head = req->get_head();

	serial_packet* pkt = m_packet_facotry.generate_packet(
		static_cast<packet_type>(head->type),
		req->msg_data(),
		head->size);

	if (pkt)
	{
		proc_packet(req->get_session(), pkt);

		return true;
	}

	return false;
}

bool net_comm::send_packet(tcp_session_ptr& sp, serial_packet& packet)
{
	if (!find_session(sp))
	{
		return false;
	}

	if (!sp->socket().is_open() || !sp->is_connected())
	{
		sp->close();
		return false;
	}

	//�����������,����װ�����л�������(Ҳ���Ƿ������)
	tcp_response_ptr resp = boost::factory<tcp_response_ptr>()(sp);  //

	generate_session_data(resp, packet);

	m_sendp.push(resp);
}

bool net_comm::send_msg(tcp_response_ptr& resp)
{
	resp->set_msg_crc();

	resp->get_session()->write(resp);
	return true;
}

//resp��׼�����������,packet�Ǵ���õ�����
void net_comm::generate_session_data(/*IN*/ tcp_response_ptr& resp, /*OUT*/ serial_packet& packet)
{
	//���л�
	char buf[4096];
	memset(buf, 0, sizeof(buf));

	std::ostrstream sa(buf,sizeof(buf));

	boost::archive::text_oarchive oa(sa);

	packet.serial(oa);     //���л��ɹ�
						   

	//2.׼��
	msg_head  head;

	head.type = packet.get_packet_type();

	std::string str(buf);

	head.size = str.size();
	head.chksum = for_each(str.begin(), str.end(), boost::crc_32_type())();   //��ÿһ���ַ���ȡ��,����CRC32����

	//3.��ֵ
	resp->get_head()->size = head.size;
	resp->get_head()->chksum = head.chksum;
	resp->get_head()->type = head.type;

	std::copy(str.begin(), str.begin() + str.size(), resp->msg_data().begin());
}

bool net_comm::add_session(const tcp_session_ptr& session)
{
	if (!find_session(session))   //���һ�»Ự�Ƿ��Ѿ�����
	{
		m_tcp_sessions.push_back(session);
		return true;
	}

	return false;
	
}

bool net_comm::remove_session(const tcp_session_ptr& session)
{
	return m_tcp_sessions.remove(session);
}


bool net_comm::find_session(const tcp_session_ptr& session)
{
	return m_tcp_sessions.find(session);

}