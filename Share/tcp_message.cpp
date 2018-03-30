#include "tcp_message.h"


tcp_message::head_data_type& tcp_message::head_data()
{
	return m_head;
}

tcp_message::body_data_type& tcp_message::msg_data()
{
	return m_msg;
}

msg_head* tcp_message::get_head()
{
	return reinterpret_cast<msg_head*>(m_head.data());
}

bool tcp_message::check_head()
{
	return (get_head()->size < MAX_BODY_SIZE);
}

void tcp_message::set_msg_crc()
{
	boost::crc_32_type crc32;
	crc32.process_bytes(&m_msg[0], get_head()->size);
	get_head()->chksum = crc32.checksum();     //设置运算出来的校验和
}


bool tcp_message::check_msg_crc()
{
	boost::crc_32_type crc32;
	crc32.process_bytes(&m_msg[0], get_head()->size);
	return get_head()->chksum == crc32.checksum();   //检查校验和是否一致
}


void tcp_message::destroy()
{
	if (m_destroy)
	{
		m_destroy(this);
	}
	else
	{
		boost::checked_delete(this);   //保证对象是完整的删除
	}
}