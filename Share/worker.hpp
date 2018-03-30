#ifndef _WORKER_H
#define _WORKER_H


/************************************************************************/
/* ����: �ܼ��岩
/* ʱ��: 2018/01/29 15:36
/* ˵��: ����������,������������й���
/*                                                                 
/************************************************************************/


template<typename Queue>
class worker
{
public:
	typedef Queue queue_type;
	typedef typename Queue::Job_type job_type;
	typedef boost::function<bool(job_type&)> func_type;   //��������<��������(�β�)> func_type xxxfunc = YYY;
	
public:
	template<typename Func>
	worker(queue_type& q,Func func ,int n = 1)
		:m_queue(q)
		,m_func(func)
		,m_nThreadNum(n)
	{
		if (m_nThreadNum < 1)
		{
			//Ĭ��ֵ
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
			//Ĭ��ֵ
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
		BOOST_ASSERT(m_func);  //���� ����Ϊ��������
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

		m_threads.join_all(); //��ס
	}
private:
	void do_work()   //ѭ������������m_queue����
	{
		while (true)
		{
			job_type job = m_queue.pop();

			//����ûд��

			if (!m_func || !m_func(job))
			{
				break;
			}
		}
	}
protected:
private:
	func_type m_func;      //�ص�������
	queue_type& m_queue;   //�����������(���ɿ���)
	int m_nThreadNum;  //�߳���
	boost::thread_group  m_threads;   //������̵߳�����std::vector<thread>
};



#endif //_WORKER_H
