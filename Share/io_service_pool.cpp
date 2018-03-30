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

//轮询获取
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
		//创建线程,run为io_service的执行函数,io_service是阻塞的所以用工作线程启动
		m_threads.create_thread(boost::bind(&ios_type::run, boost::ref(ios)));
	}
}

void io_service_pool::run()
{
	start();
	//等待
	m_threads.join_all();
}

void io_service_pool::stop()
{
	m_works.clear();

	//把每个io_service都停止一下
	std::for_each(m_io_services.begin(), m_io_services.end(), boost::bind(&ios_type::stop, _1));
}

void io_service_pool::init(int n)
{
	for (int i = 0;i < n;++i)
	{
		//装指针的vector      
		m_io_services.push_back(boost::factory<ios_type*>()());    //重载了()的方法

		//放进works对象里面
		m_works.push_back(boost::factory<work_type*>()(m_io_services.back()));
	}
}