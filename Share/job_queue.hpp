#ifndef _JOB_QUEUE_H
#define _JOB_QUEUE_H

#include <queue>
#include <boost/noncopyable.hpp>   //用于禁止拷贝
#include <boost/thread.hpp>
#include <boost/concept_check.hpp>   //概念检查
#include <boost/utility/value_init.hpp>

template<typename Job>
class job_queue : public boost::noncopyable
{
public:
	typedef Job Job_type;
	typedef std::queue<Job_type> queue_type;
	typedef boost::mutex mutex_type;
	typedef typename mutex_type::scoped_lock lock_type;

	typedef boost::condition_variable_any  condition_type;   //条件变量

	BOOST_CONCEPT_ASSERT((boost::SGIAssignable<Job_type>));   //要求有拷贝构造函数和重载operator=  传入进来的Job类型
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<Job_type>));   //要求有缺省默认构造
public:
	job_queue()
		:m_stop_flag(false)
	{

	}

	void push(const Job_type& x)
	{
		lock_type lock(m_mutex);    //上锁
		m_queue.push(x);           //入队
		m_hasJob.notify_one();   //通知 <-----m_hasJob被赋为真
	}

	Job_type pop()
	{
		lock_type lock(m_mutex);
		while (m_queue.empty() && !m_stop_flag)
		{
			//等待
			m_hasJob.wait(m_mutex);
		}

		if (m_stop_flag)
		{
			//返回的是Job_type的临时对象
			return boost::initialized_value;
		}

		if (m_queue.empty())
		{
			return boost::initialized_value;
		}

		Job_type tmp = m_queue.front();   //从队头出

		m_queue.pop();   //从队列里移除

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


