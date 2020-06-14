// File name:   composite_tree.hpp
// Developer:   Type Your Name Here
// Date Created: 28/10/2019 20:52
// Last Modified: 28/10/2019 20:52
// Description: Tree files directories container using composite pattern.

#ifndef __ILRD_COMPOSITE_TREE_HPP__
#define __ILRD_COMPOSITE_TREE_HPP__

#include <string>   //string
#include <vector>   //vector
#include <memory>   //shared_ptr
#include <dirent.h> // diropen
#include <sys/types.h> // diropen
#include <cstring> // strcmp
#include <algorithm> // for_each

namespace ilrd
{

class DirGuard
{
public:
	DirGuard(std::string path)
	{
		m_dir = opendir(path.c_str()); 
	}
	~DirGuard()
	{
		closedir(m_dir);
	}
	DIR* get()
	{
		return m_dir;
	}
private:
	DIR* m_dir;
};

class File: public std::enable_shared_from_this<File>
{
public:
    explicit File(const std::string& name);
    virtual ~File() = default;
    
    File(const File& other) = default;
    File& operator=(const File& other) = default;

    virtual std::shared_ptr<const File> find(const std::string& name) const noexcept;

    virtual void display(std::ostream& os) const; 
private:
    std::string m_name;
};

class Directory : public File
{
public:
    static std::shared_ptr<const Directory> create(const std::string& path); 
    std::shared_ptr<const File> find(const std::string& name) const noexcept override; 
    void display(std::ostream& os) const override; 
private:
    std::vector<std::shared_ptr<const File>> m_files;
    explicit Directory(const std::string& name);
 
    void add(std::shared_ptr<const File> entry);
};

class FindFunctor
{
public:
	FindFunctor(const std::string& name)
		: m_name(name), m_found(0) { }
	void operator()(std::shared_ptr<const File> file)
	{
		if(m_found == 0 && file->find(m_name)) // once you find the first appearence,
												// don't search anymore
		{
			m_found = file->find(m_name);
		}
	}
	
	std::shared_ptr<const File> get()
	{
		return m_found;
	}

private:
	std::string m_name;
	std::shared_ptr<const File> m_found;
};

class DisplayFunctor
{
public:
	DisplayFunctor(std::ostream& os)
		: m_os(os) { }
	void operator()(std::shared_ptr<const File> file)
	{
		file->display(m_os);
	}
private:
	std::ostream& m_os;
};

}   //namespace ilrd

#endif //__ILRD_COMPOSITE_TREE_HPP__
