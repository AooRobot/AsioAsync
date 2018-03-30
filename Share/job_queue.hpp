#ifndef _JOB_QUEUE_H
#define _JOB_QUEUE_H

#include <queue>
#include <boost/noncopyable.hpp>   //���ڽ�ֹ����
#include <boost/thread.hpp>
#include <boost/concept_check.hpp>   //������
#include <boost/utility/value_init.hpp>

template<typename Job>
class job_queue : public boost::noncopyable
{
public:
	typedef Job Job_type;
	typedef std::queue<Job_type> queue_type;
	typedef boost::mutex mutex_type;
	typedef typename mutex_type::scoped_lock lock_type;

	typedef boost::condition_variable_any  condition_type;   //��������

	BOOST_CONCEPT_ASSERT((boost::SGIAssignable<Job_type>));   //Ҫ���п������캯��������operator=  ���������Job����
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<Job_type>));   //Ҫ����ȱʡĬ�Ϲ���
public:
	job_queue()
		:m_stop_flag(false)
	{

	}

	void push(const Job_type& x)
	{
		lock_type lock(m_mutex);    //����
		m_queue.push(x);           //���
		m_hasJob.notify_one();   //֪ͨ <-----m_hasJob����Ϊ��
	}

	Job_type pop()
	{
		lock_type lock(m_mutex);
		while (m_queue.empty() && !m_stop_flag)
		{
			//�ȴ�
			m_hasJob.wait(m_mutex);
		}

		if (m_stop_flag)
		{
			//���ص���Job_type����ʱ����
			return boost::initialized_value;
		}

		if (m_queue.empty())
		{
			return boost::initialized_value;
		}

		Job_type tmp = m_queue.front();   //�Ӷ�ͷ��

		m_queue.pop();   //�Ӷ������Ƴ�

		return tmp;
	}

	void stop()
	{
		m_stop_flag = true;
		m_hasJob.notify_all();
	}

private:
	queue_type m_queue;
	mutex_type m_mutex;
	condition_type m_hasJob;
	bool m_stop_flag;
};



#endif //_JOB_QUEUE_H


