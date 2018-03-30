#ifndef  _IO_SERVICE_POOL_H
#define  _IO_SERVICE_POOL_H

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/thread.hpp>
#include <boost/functional/factory.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

class io_service_pool :public boost::noncopyable   //���Ժ��ж��io_service  '��Ƶ�'
{
public:
	typedef boost::asio::io_service  ios_type;			// io��������  �첽���񹤾�
	typedef boost::asio::io_service::work  work_type;   //ȷ��io_service��������
	typedef boost::ptr_vector<ios_type>  io_services_type;
	typedef boost::ptr_vector<work_type>  works_type;

public:
	explicit io_service_pool(int n = 4);    //explicit ֻ��ͨ�����ַ�ʽ���������,û�����صĹ��췽��

	ios_type& get();  //��ѯ����,���ⲿ��ȡio_service����,�������acceptor����

	void start();    //�����̳߳�
	void stop();	 //ֹͣ�̳߳�
	void run();		 //�̳߳�����

protected:
private:
	io_services_type  m_io_services;
	works_type  m_works;   //��֤io_services����������

	boost::thread_group  m_threads;    //�߳���
	std::size_t  m_next_io_services;   //������ѯʹ��

	void init(int n);    //����һϵ��io_services���󲢼��뵽m_io_services
};

#endif // _IO_SERVICE_POOL_H
