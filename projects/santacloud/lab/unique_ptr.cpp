#include <iostream>
#include <memory>


// try creating a cctor in a class that has a unique_ptr as a member,
// can i std::move it? turns out you can't


class CopyClass
{
public:
CopyClass() { }

CopyClass(CopyClass&& other)
{
	std::cout << "mctor" << std::endl;
}
private:
	std::unique_ptr<int> m_ptr;
};

class Task
{
public:

	Task(std::unique_ptr<int> ptr)
		: m_ptr(ptr.release())
	{
		std::cout << "got " << *m_ptr << std::endl;
		
	}
private:
	std::unique_ptr<int> m_ptr;
};

class TaskDerived : public Task
{
public:
	TaskDerived(std::unique_ptr<int> ptr)
		: Task(std::move(ptr)) { }
};

std::unique_ptr<Task> foo(std::unique_ptr<int> uptr)
{
	std::cout << "here" << std::endl;
	std::cout << *uptr << std::endl;

	std::unique_ptr<Task> task(new TaskDerived(std::move(uptr)));
	return task;
	
	//TaskDerived(std::forward<std::unique_ptr<int>>(uptr));
	//Task t(std::forward<std::unique_ptr<int>>(uptr));
	
}


// 7.12 - i want to check if i can return from a function an instance
// of CopyClass like i can return a unique_ptr since it's an rvalue.
// turns out you can.
CopyClass ret_copy_class()
{
	return CopyClass();
}

// 25.12 - i wanna check if i can copy unique_ptr to shared_ptr
void shared()
{
	std::unique_ptr<int> uptr(new int(3));
	
	std::shared_ptr<int> sptr(uptr.get());
}

int main()
{
	std::unique_ptr<int> uptr(new int(3));
	
	shared();
	//std::unique_ptr<int> uuptr;
	//uuptr.reset(uptr.release());
	//foo(std::move(uptr));
	
	//CopyClass c = ret_copy_class();
	
	return 0;
}
