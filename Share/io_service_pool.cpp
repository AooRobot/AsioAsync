#include "io_service_pool.h"


io_service_pool::io_service_pool(int n)
	:m_next_io_services(0)
{
	if (n <= 0)
	{
		n = 1;
	}

	init(n);
}

//��ѯ��ȡ
io_service_pool::ios_type& io_service_pool::get()
{
	if (m_next_io_services >= m_io_services.size())
	{
		m_next_io_services = 0;
	}

	return m_io_services[m_next_io_services++];
}

void io_service_pool::start()
{
	if (m_threads.size() > 0)
	{
		return;
	}

	BOOST_FOREACH(ios_type& ios, m_io_services)
	{
		//�����߳�,runΪio_service��ִ�к���,io_service�������������ù����߳�����
		m_threads.create_thread(boost::bind(&ios_type::run, boost::ref(ios)));
	}
}

void io_service_pool::run()
{
	start();
	//�ȴ�
	m_threads.join_all();
}

void io_service_pool::stop()
{
	m_works.clear();

	//��ÿ��io_service��ֹͣһ��
	std::for_each(m_io_services.begin(), m_io_services.end(), boost::bind(&ios_type::stop, _1));
}

void io_service_pool::init(int n)
{
	for (int i = 0;i < n;++i)
	{
		//װָ���vector      
		m_io_services.push_back(boost::factory<ios_type*>()());    //������()�ķ���

		//�Ž�works��������
		m_works.push_back(boost::factory<work_type*>()(m_io_services.back()));
	}
}