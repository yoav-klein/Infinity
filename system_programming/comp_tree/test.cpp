#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <memory>
#include <algorithm>
#include <vector>

class Base
{
public:
	virtual void SayHey()
	{
		std::cout << "Base hey" << std::endl;
	}
};

class Derived : public Base
{
	void SayHey() override
	{
		std::cout << "Derived hey" << std::endl;
	}
};

class Dog
{
public:
	static void AccessToPrivate();
	void Bark() { std::cout << "Woof" << std::endl; }
	void Change() { m_a = 2; }
private:
	int m_a;
};

void Dog::AccessToPrivate()
{
	Dog d;
	std::cout << d.m_a;
}

class DirGuard
{
public:
	DirGuard(const std::string& path)
	{
		m_dir = opendir(path.c_str()); 
	}
	
	DIR* get()
	{
		return m_dir;
	}
	
	~DirGuard()
	{
		closedir(m_dir);
	}
	
	
private:
	DIR* m_dir;
};


void Recursive(const std::string& path)
{
	DirGuard dir(path);
	struct dirent* entry;
	
	while(entry = readdir(dir.get()))
	{
		std::string temp_path = path;
		temp_path.append("/").append(entry->d_name);
		std::cout << temp_path << std::endl;
		DirGuard is_dir(temp_path);
		if(strcmp(".", entry->d_name) && strcmp(entry->d_name, "..") && is_dir.get())
		{
			std::cout << "Here" << std::endl;
			Recursive(temp_path);
		}
	}
	
}

void CallSayHey(Base* b)
{
	b->SayHey();
}

void ForEach()
{
	std::vector<Base*> vec;
	vec.push_back(new Base());
	vec.push_back(new Base());
	vec.push_back(new Derived());
	vec.push_back(new Base());
	
	for_each(vec.begin(), vec.end(), CallSayHey);
}

void Const()
{
	//std::shared_ptr<const Dog> d(new Dog());
	
	//d->Change();
	
	//Dog::AccessToPrivate();
	
	ForEach();
}

void Rfind()
{
	std::string s = "Yoav/Klein";
	s.rfind("/");
}

int main()
{
	std::string path = "dir";
	//Recursive(path);
	
	//Const();
	Rfind();
	
	return 0;
}
