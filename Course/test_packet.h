#ifndef _TEST_PACKET_H
#define _TEST_PACKET_H

#include "serial_packet.h"

class test_packet : public serial_packet
{
	_SERIALIZATION_PACKET()

public:
	test_packet()
		:serial_packet(PACKET_TEST)   //������
	{

	}
protected:
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_strUserName & m_strPassWord & m_vStr;
	}

public:
	std::string m_strUserName;
	std::string m_strPassWord;
	std::vector<std::string> m_vStr;
};

void print_test_packet(const test_packet& packet)
{
	std::cout << "�˺�:" << packet.m_strUserName << std::endl;
	std::cout << "����:" << packet.m_strPassWord << std::endl;

	for (int i = 0; i < packet.m_vStr.size(); i++)
	{
		std::cout << packet.m_vStr[i] << std::endl;
	}
}

void my_test_packet()
{
	test_packet set;

	set.m_strUserName = "110114";
	set.m_strPassWord = "95144";

	set.m_vStr.push_back("8888");
	set.m_vStr.push_back("6666");
	set.m_vStr.push_back("������Ϣ");
	set.m_vStr.push_back("CPU��Ϣ");
	set.m_vStr.push_back("�Կ�����");

	char buf[1024];
	memset(buf, 0, sizeof(buf));

	std::ostrstream sa(buf, 1024);

	text_oarchive oa(sa);

	set.serial(oa);   //���л��ɹ�

	//------------
	test_packet get;
	std::stringstream ss(buf);

	text_iarchive ia(ss);

	get.unserial(ia);    //�����л��ɹ�

	print_test_packet(get);
}

class login_packet : public serial_packet
{
	_SERIALIZATION_PACKET()

public:
	login_packet()
		:serial_packet(PACKET_LOGIN)   //������
	{

	}

	void print()
	{
		std::cout << "\n-----------------\n"
			<< "login_packet\n"
			<< m_strUserName << std::endl
			<< m_strUserName << std::endl
			<< "-----------------\n";
	}

protected:
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_strUserName & m_strPassWord;
	}



public:
	std::string m_strUserName;
	std::string m_strPassWord;
	
};

class acklogin_packet : public serial_packet
{
	_SERIALIZATION_PACKET()

public:
	acklogin_packet()
		:serial_packet(PACKET_ACK_LOGIN)   //������
	{

	}

	void print()
	{
		std::cout << "\n-----------------\n"
			<< "acklogin_packet\n"
			<< m_strUserName << std::endl
			<< m_strUserName << std::endl
			<< m_strAck << std::endl
			<< "-----------------\n";
	}

protected:
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_strUserName & m_strPassWord & m_strAck;
	}



public:
	std::string m_strUserName;
	std::string m_strPassWord;
	std::string m_strAck;
};


#endif // _TEST_PACKET_H
