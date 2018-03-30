#ifndef _HEART_PACKET_H
#define _HEART_PACKET_H

#include "../Course/serial_packet.h"

class heart_packet : public serial_packet
{
	_SERIALIZATION_PACKET()

public:
	heart_packet()
		:serial_packet(PACKET_HEART)   //ÐÄÌøÓÃ
	{

	}

protected:
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_hour & m_min & m_sec & m_milsec;
	}

public:
	int m_hour;
	int m_min;
	int m_sec;
	int m_milsec;
};



#endif // _HEART_PACKET_H
