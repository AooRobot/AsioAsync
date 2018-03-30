#ifndef _SAFE_VECTOR_H
#define _SAFE_VECTOR_H

#include "safe_container.hpp"
#include <vector>


template<typename T>
class safe_vector : public safe_container
{
	typedef std::vector<T> vector_type;
public:
	typedef typename vector_type::value_type		value_type;  //萃取
	typedef typename vector_type::pointer		pointer_type;
	typedef typename vector_type::const_pointer	const_pointer;
	typedef typename vector_type::size_type		size_type;
	typedef typename vector_type::reference		reference;
	typedef typename vector_type::const_reference const_reference;

	typedef typename vector_type::iterator  iterator;
	typedef typename vector_type::const_iterator  const_iterator;

	typedef typename vector_type::reverse_iterator  reverse_iterator;
	typedef typename vector_type::const_reverse_iterator  const_reverse_iterator;

	typedef boost::optional<vector_type> optional_vector_type;   //主要重载=操作符

public:
	bool empty()
	{
		read_lock lock(m_mutex);  //线程安全 上读锁
		return m_vector.empty();
	}

	size_t size() const
	{
		read_lock lock(m_mutex);    //锁的状态会变化
		return m_vector.size();
	}

	size_t max_size() const
	{
		read_lock lock(m_mutex);
		return m_vector.max_size();
	}

	iterator begin()
	{
		read_lock lock(m_mutex);
		return m_vector.begin();
	}

	const_iterator begin() const
	{
		read_lock lock(m_mutex);
		return m_vector.begin();
	}

	iterator end()
	{
		read_lock lock(m_mutex);
		return m_vector.end();
	}

	const_iterator end() const
	{
		read_lock lock(m_mutex);
		return m_vector.end();
	}

	bool find(const T& value)   //仅仅做查找，看看这个元素是否在里面
	{
		read_lock lock(m_mutex);
		vector_type::const_iterator iter = m_vector.begin();   //原来的iter
		for (;iter != m_vector.end();++iter)    //xx++  会产生拷贝
		{
			if (*iter == value)
			{
				return true;
			}
		}
		return false;
	}

	const T& operator[](const int k)
	{
		read_lock lock(m_mutex);
		return m_vector[k];
	}

	void push_back(const T& value)
	{
		write_lock lock(m_mutex);	//写入操作 上写锁
		m_vector.push_back(value);
	}

	template<typename Func>
	void for_each(Func func)
	{
		read_lock lock(m_mutex);
		std::for_each(m_vector.begin(); m_vector.end(); func);   //每一个函数都执行func函数
	}

	void clear()
	{
		write_lock lock(m_mutex);
		m_vector.clear();
	}

	void erase(const_iterator _where)
	{
		write_lock lock(m_mutex);
		m_vector.erase(_where);
	}

	bool remove(const T& value)
	{
		write_lock lock(m_mutex);

		vector_type::const_iterator iter = m_vector.begin();  
		for (; iter != m_vector.end();)    
		{
			if (*iter == value)
			{
				iter = m_vector.erase(iter);   //擦除erase()会返回下一个数据的指针
				return true;
			}
			else
			{
				++iter;
			}
		}
		return false;
	}

protected:
private:
	vector_type m_vector;
};



#endif //_SAFE_VECTOR_H
