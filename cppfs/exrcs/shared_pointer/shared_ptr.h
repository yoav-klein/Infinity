#ifndef __SHARED_PTR_H__
#define __SHARED_PTR_H__

namespace ilrd
{

template<typename T>
class SharedPtr
{
public:
	
	explicit SharedPtr(T* ptr = 0);
	~SharedPtr();
	
	SharedPtr(const SharedPtr<T>& other);
	SharedPtr<T>& operator=(const SharedPtr<T>& other);
	
	template<typename O>
	SharedPtr<T>& operator=(const SharedPtr<O>& other);
	
	template<typename O>
	SharedPtr(const SharedPtr<O>& other);
	
	T& operator*() const;
	T* operator->() const;
	
	bool operator!() const;
	T* GetPtr() const;
	
	
private:
	template<typename O>
	friend class SharedPtr;
	
	void DeleteIfShould();
	T* m_ptr;
	std::size_t* m_count_ref;
};

template<typename T>
void SharedPtr<T>::DeleteIfShould()
{
	--*m_count_ref;
	if(!*m_count_ref)
	{
		delete m_ptr;
		delete m_count_ref;
	}
}

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr)
	: m_ptr(ptr), m_count_ref(new std::size_t(1))
{
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
	DeleteIfShould();
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
	: m_ptr(other.m_ptr), m_count_ref(other.m_count_ref)
{
	++*m_count_ref;
}

template<typename T>
template<typename O>
SharedPtr<T>::SharedPtr(const SharedPtr<O>& other)
	: m_ptr(other.GetPtr()), m_count_ref(other.m_count_ref)
{
	++*m_count_ref;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
	if(other.m_ptr == m_ptr)
	{
		return *this;
	}
	
	DeleteIfShould();
	
	m_ptr = other.m_ptr;
	m_count_ref = other.m_count_ref;
	++*m_count_ref;
	
	return *this;
}

template <typename T>
template <typename O>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<O>& other)
{
	return *this = SharedPtr<T>(other);
}

template<typename T>
T& SharedPtr<T>::operator*() const
{
	return *m_ptr;
}

template<typename T>
T* SharedPtr<T>::operator->() const
{
	return m_ptr;
}

template<typename T>
bool SharedPtr<T>::operator!() const
{
	return (!m_ptr);
}

template<typename T>
T* SharedPtr<T>::GetPtr() const
{
	return m_ptr;
}


} // namespace ilrd

#endif // __SHARED_PTR_H__
