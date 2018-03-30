#ifndef _SAFE_VECTOR_H
#define _SAFE_VECTOR_H

#include "safe_container.hpp"
#include <vector>


template<typename T>
class safe_vector : public safe_container
{
	typedef std::vector<T> vector_type;
public:
	typedef typename vector_type::value_type		value_type;  //��ȡ
	typedef typename vector_type::pointer		pointer_type;
	typedef typename vector_type::const_pointer	const_pointer;
	typedef typename vector_type::size_type		size_type;
	typedef typename vector_type::reference		reference;
	typedef typename vector_type::const_reference const_reference;

	typedef typename vector_type::iterator  iterator;
	typedef typename vector_type::const_iterator  const_iterator;

	typedef typename vector_type::reverse_iterator  reverse_iterator;
	typedef typename vector_type::const_reverse_iterator  const_reverse_iterator;

	typedef boost::optional<vector_type> optional_vector_type;   //��Ҫ����=������

public:
	bool empty()
	{
		read_lock lock(m_mutex);  //�̰߳�ȫ �϶���
		return m_vector.empty();
	}

	size_t size() const
	{
		read_lock lock(m_mutex);    //����״̬��仯
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

	bool find(const T& value)   //���������ң��������Ԫ���Ƿ�������
	{
		read_lock lock(m_mutex);
		vector_type::const_iterator iter = m_vector.begin();   //ԭ����iter
		for (;iter != m_vector.end();++iter)    //xx++  ���������
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
		write_lock lock(m_mutex);	//д����� ��д��
		m_vector.push_back(value);
	}

	template<typename Func>
	void for_each(Func func)
	{
		read_lock lock(m_mutex);
		std::for_each(m_vector.begin(); m_vector.end(); func);   //ÿһ��������ִ��func����
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
				iter = m_vector.erase(iter);   //����erase()�᷵����һ�����ݵ�ָ��
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
