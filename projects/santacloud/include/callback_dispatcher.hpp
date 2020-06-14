#ifndef __CALLBACK_DISPATCHER_HPP__
#define __CALLBACK_DISPATCHER_HPP__

#include <iostream>

#include <algorithm> // std::for_each
#include <unordered_set> // std::unordered_set
#include <vector> // std::vector

/********************************************
	Dispatcher-Callback
	
	this is an extension of the publisher-subscriber dp.
	we have an template class of Dispatcher that is templated
	on the type of the message it passes to the subscribers,
	in our case - Callback objects, which are templated on the
	same message type. a Dispatcher has a container of CallbackBase objects
	that it notifies on each modification. 
	
	CallbackBase is an interface that has a 'notify' function,
	which is implemented by it's derived Callback class,
	which, in turn, is templated on the type of the class
	containing it. the reason for this is that the Callback object
	contains a reference to the object that contains it (client), because
	it needs to call the client's do_action method on notification.
	
	CallbackBase needs to call 'add' of Dispatcher to subscribe itself
	to the container of objects being notified, and remove upon 
	going out of scope.

**********************************************/

template <typename Msg>
class CallbackBase;

template <typename Msg>
class Dispatcher
{
public:
    Dispatcher() = default;

    // must notify all of the viewers on destruction
    ~Dispatcher();
    
    Dispatcher(const Dispatcher&)            = delete; 
    Dispatcher(Dispatcher&&)                 = delete; 
    
    Dispatcher& operator=(const Dispatcher&) = delete; 
    Dispatcher& operator=(Dispatcher&&)      = delete; 
  
    void notify_all(const Msg& msg);

private:
    std::unordered_set<CallbackBase<Msg>*> m_subscribers; 

    friend class CallbackBase<Msg>;
    
    // adds new subscriber
    void add(CallbackBase<Msg>* callback);
    void remove(CallbackBase<Msg>* callback);
    void notify_out_of_service();
    
    // remove pushes to this vector, actual erasing is on notify_all
    std::vector<CallbackBase<Msg>*> subs_to_remove;
};

template <typename Msg>
class CallbackBase
{
public:
    explicit CallbackBase(Dispatcher<Msg>* dispatcher);
    
    // on dtor, calling remove from Dispatcher. 
    
    virtual ~CallbackBase() = 0;
    
    CallbackBase(const CallbackBase&)            = delete;
    CallbackBase(CallbackBase&&)                 = delete; 
    
    CallbackBase& operator=(const CallbackBase&) = delete; 
    CallbackBase& operator=(CallbackBase&&)      = delete; 

private:
    Dispatcher<Msg>* m_dispatcher; 
	bool m_is_dispatcher_exist;
    virtual void notify(const Msg& msg) = 0;
    virtual void out_of_service() = 0;

    friend class Dispatcher<Msg>;
};


template <typename Msg, typename Observer>
class Callback : public CallbackBase<Msg>
{
public:
    //ensure member function
    using ActionMethod = void (Observer::*)(const Msg&);
    using StopMethod = void (Observer::*)();

    explicit Callback(Dispatcher<Msg>* dispatcher, 
                      Observer& observer, 
                      ActionMethod action_method, 
                      StopMethod stop_method = nullptr);
    ~Callback() = default;
    
    Callback(const Callback&)            = delete; 
    Callback(Callback&&)                 = delete; 
    
    Callback& operator=(const Callback&) = delete; 
    Callback& operator=(Callback&&)      = delete; 
    
        
private:
    void notify(const Msg& msg) override;
    void out_of_service() override;

    Observer& m_observer;
    ActionMethod m_action_method;
    StopMethod m_stop_method;

};


template <typename Msg>
void Dispatcher<Msg>::add(CallbackBase<Msg>* callback)
{
	m_subscribers.insert(callback);
}

template <typename Msg>
void Dispatcher<Msg>::remove(CallbackBase<Msg>* callback)
{
	subs_to_remove.push_back(callback);
}


template <typename Msg>
void Dispatcher<Msg>::notify_all(const Msg& msg)
{
	for_each(subs_to_remove.begin(), subs_to_remove.end(), // remove subscribers that were deleted
	[=](CallbackBase<Msg>* callback)
	{
		m_subscribers.erase(callback);
	});
	
	subs_to_remove.clear();
	
	for(auto iter = m_subscribers.begin(); iter != m_subscribers.end(); ++iter)
	{
		(*iter)->notify(msg);
	}
}	


template <typename Msg>
void Dispatcher<Msg>::notify_out_of_service()
{
	for_each(subs_to_remove.begin(), subs_to_remove.end(), // remove subscribers that were deleted
	[=](CallbackBase<Msg>* callback)
	{
		m_subscribers.erase(callback);
	});
	
	for(auto iter = m_subscribers.begin(); iter != m_subscribers.end(); ++iter)
	{
		(*iter)->m_is_dispatcher_exist = false;
		(*iter)->out_of_service();
	}
}

template <typename Msg>
Dispatcher<Msg>::~Dispatcher()
{
	notify_out_of_service();
}

template <typename Msg>
CallbackBase<Msg>::CallbackBase(Dispatcher<Msg>* dispatcher)
	: m_dispatcher(dispatcher), m_is_dispatcher_exist(true)
{
	dispatcher->add(this);
}

template <typename Msg>
CallbackBase<Msg>::~CallbackBase()
{
	if(m_is_dispatcher_exist)
	{
		m_dispatcher->remove(this);
	}
}

template <typename Msg, typename Observer>
Callback<Msg, Observer>::Callback(Dispatcher<Msg>* dispatcher, 
                      Observer& observer, 
                      ActionMethod action_method, 
                      StopMethod stop_method)
	: CallbackBase<Msg>(dispatcher), m_observer(observer),
	  m_action_method(action_method), m_stop_method(stop_method)
{
}


template <typename Msg, typename Observer>
void Callback<Msg, Observer>::notify(const Msg& msg)
{
	(m_observer.*m_action_method)(msg);
}

template <typename Msg, typename Observer>
void Callback<Msg, Observer>::out_of_service()
{
	if(nullptr != m_action_method)
	{
		(m_observer.*m_stop_method)();
	}
}
#endif // __CALLBACK_DISPATCHER_HPP__
