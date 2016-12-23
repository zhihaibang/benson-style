#ifndef _SINGLETON_H_
#define _SINGLETON_H_


template<typename TYPE>
class Singleton
{
public:
	static TYPE * instance();
	static void free();
private:
	static TYPE * m_ins;
};

template<typename TYPE>
TYPE * Singleton<TYPE>::m_ins = 0; 

template<typename TYPE>
TYPE * Singleton<TYPE>::instance()
{
	if(0 == m_ins)
	{
		m_ins = new TYPE;
	}
	return m_ins;
}

template<typename TYPE>
void Singleton<TYPE>::free()
{
	if(m_ins)
	{
		delete m_ins;
	}
	m_ins = 0;
}

#endif

