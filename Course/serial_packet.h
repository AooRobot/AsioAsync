#ifndef _SERIAL_PACKET_H
#define _SERIAL_PACKET_H

#include "packet_type.h"

#include <iostream>
#include <strstream>
#include <sstream>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>    //输入用
#include <boost/archive/text_oarchive.hpp>    //输出用
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <boost/iostreams/stream.hpp>


using namespace boost::archive;

// 斜杠之后不能有空格
#define _SERIALIZATION_PACKET()\
public:\
	virtual void serial(text_oarchive& oa)\
	{\
		oa << *this;\
	}\
	virtual void unserial(text_iarchive& ia)\
	{\
		ia >> *this;\
	}\
	int get_packet_type()\
	{\
		return static_cast<int>(m_packet_type);\
	}\
protected:\
	friend boost::serialization::access;  


class serial_packet
{
	_SERIALIZATION_PACKET()
public:
	serial_packet(packet_type pkt_type)
		:m_packet_type(pkt_type)
	{

	}

protected:
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_packet_type;
	}

protected:
	packet_type m_packet_type;
};








#endif //_SERIAL_PACKET_H
