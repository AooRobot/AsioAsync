#ifndef _PACKET_FACTORY_H
#define _PACKET_FACTORY_H

#include "packet_type.h"
#include "../Share/msg_def.h"
#include <map>

#include <boost/array.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/functional/factory.hpp>

class serial_packet;

typedef char char_type;
typedef boost::array<char_type, MAX_BODY_SIZE> data_type;
typedef std::size_t  size_type;



template<typename Packet>
serial_packet* create_packet(data_type& data, size_type size)  //解整编(反序列化)
{
	std::string str_data(data.data(), size);

	std::stringstream ss(str_data);

	boost::archive::text_iarchive ia(ss);

	Packet* pkt = boost::factory<Packet*>()();

	pkt->unserial(ia);

	return pkt;
}

//typedef serial_packet* (*packet_creator)(data_type&, size_type);  //C语言的函数指针
typedef std::function<serial_packet* (data_type&, size_type)> packet_creator;


class packet_factory
{
public:
	typedef std::map<packet_type, packet_creator> packet_creators;
public:

	template<typename Packet>
	bool register_packet_type(packet_type type)
	{
		return m_mpCreators.insert(
			std::make_pair(type, create_packet<Packet>)).second;
	}

	serial_packet* generate_packet(packet_type type, data_type& data, size_type size)
	{
		packet_creators::iterator iter = m_mpCreators.find(type);

		if (iter!= m_mpCreators.end())
		{
			return iter->second(data, size);    //反序列化
		}
		
		return nullptr;
	}

	const packet_creators& get_creator() const
	{
		return m_mpCreators;
	}
protected:
private:
	packet_creators  m_mpCreators;
};








#endif // _PACKET_FACTORY_H
