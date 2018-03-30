#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <vector>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>

class scheduler
{
public:
	typedef boost::function<void()> func_type;
	typedef std::pair<func_type, int> scheduler_type;

	typedef std::vector<scheduler_type> scheduler_funcs;
public:
	void add(func_type func,int seconds)
	{
		m_funcs.push_back(std::make_pair(func, seconds));
	}

	void start()
	{
		BOOST_FOREACH(scheduler_funcs::reference x,m_funcs)
		{
			m_threads.create_thread(
					boost::bind(&scheduler::schedule,
						this,
						x.first,
						x.second
						)
			);
		}
	}

	void run()
	{
		start();
		m_threads.join_all();
	}
protected:
	void schedule(func_type func,int sec)
	{
		while (true)
		{
			func();
			boost::this_thread::sleep(boost::posix_time::seconds(sec));
		}
	}
private:
	scheduler_funcs m_funcs;   //装的都是回调函数

	boost::thread_group m_threads;
};

#endif // _SCHEDULER_H

