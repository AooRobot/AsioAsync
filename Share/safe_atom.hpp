#ifndef _SAFE_ATOM_H
#define _SAFE_ATOM_H

#include <boost/thread.hpp>

template<typename T>
class safe_atom
{
public:
	typedef boost::shared_mutex mutex_type;

	safe_atom(T v);


	void setValue(const T& v);   //互斥体只能用引用方式传递
	void getValue(T& v);
private:
	mutex_type m_mu;   //定义一把锁
	T value;
};


template<typename T>
safe_atom<T>::safe_atom(T v)
	:value(v)
{

}

template<typename T>
void safe_atom<T>::setValue(const T& v)
{
	//上锁
	boost::unique_lock<boost::shared_mutex> lock(m_mu);
	value = v;
}

template<typename T>
void safe_atom<T>::getValue(T& v)
{
	//上锁
	boost::unique_lock<boost::shared_mutex> lock(m_mu);
	v = value;
}


#endif // _SAFE_ATOM_H
