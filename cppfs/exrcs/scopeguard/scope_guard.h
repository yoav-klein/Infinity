#ifndef __SCOPE_GUARD_H__
#define __SCOPE_GUARD_H__

template<typename T, void(T::*acquire)(), void(T::*release)()>
class ScopeGuard
{
public:
	ScopeGuard(T& resource)
		: m_resource(&resource), m_flag(1)
	{
		Acquire();
	}
	
	~ScopeGuard()
	{
		Release();
	}
	
	void Acquire()
	{
		(m_resource->*acquire)();
		m_flag = 1;
	}
	
	void Release()
	{
		if(-1 != m_flag)
		{
			m_flag = -1;
			(m_resource->*release)();
		}
	}

private:
	T* m_resource;
	int m_flag;
	ScopeGuard(const ScopeGuard& other); // uncopyable
	ScopeGuard& operator=(const ScopeGuard& other); // uncopyable;
};

#endif // __SCOPE_GUARD_H__
