#ifndef _WORKER_H
#define _WORKER_H


/************************************************************************/
/* 作者: 曹计彦博
/* 时间: 2018/01/29 15:36
/* 说明: 工作工厂类,处理对列中所有工作
/*                                                                 
/************************************************************************/


template<typename Queue>
class worker
{
public:
	typedef Queue queue_type;
	typedef typename Queue::Job_type job_type;
	typedef boost::function<bool(job_type&)> func_type;   //函数类型<返回类型(形参)> func_type xxxfunc = YYY;
	
public:
	template<typename Func>
	worker(queue_type& q,Func func ,int n = 1)
		:m_queue(q)
		,m_func(func)
		,m_nThreadNum(n)
	{
		if (m_nThreadNum < 1)
		{
			//默认值
			m_nThreadNum = 1;
		}
	}

	worker(queue_type& q,int n = 1)
		:m_queue(q)
		,m_func(nullptr)
		,m_nThreadNum(n)
	{
		if (m_nThreadNum < 1)
		{
			//默认值
			m_nThreadNum = 1;
		}
	}

	template<typename Func>
	void set_workFunc(Func func)
	{
		m_func = func;
	}

	void start()
	{
		BOOST_ASSERT(m_func);  //断言 必须为真否则崩溃
// 		if (!m_func)
// 		{
// 			return;
// 		}

		if (m_threads.size() > 0)
		{
			return;
		}

		for (int i = 0;i < m_nThreadNum;++i)
		{
			m_threads.create_thread(boost::bind(&worker::do_work, this));
		}
	}

	void stop()
	{
		m_func = 0;
		m_queue.stop();
	}

	void run()
	{
		this->start();

		m_threads.join_all(); //卡住
	}
private:
	void do_work()   //循环处理工作队列m_queue函数
	{
		while (true)
		{
			job_type job = m_queue.pop();

			//函数没写完

			if (!m_func || !m_func(job))
			{
				break;
			}
		}
	}
protected:
private:
	func_type m_func;      //回调处理函数
	queue_type& m_queue;   //关联任务对列(不可拷贝)
	int m_nThreadNum;  //线程数
	boost::thread_group  m_threads;   //管理多线程的容器std::vector<thread>
};



#endif //_WORKER_H
