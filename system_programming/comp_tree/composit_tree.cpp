#include <iostream> // std::endl

#include "composit_tree.hpp"

namespace ilrd
{

File::File(const std::string& name)
	: m_name(name) { }


std::shared_ptr<const File> File::find(const std::string& name) const noexcept
{
	if(!strcmp(&m_name[m_name.rfind('/') + 1], name.c_str()))
	{
		return shared_from_this();
	}
	return 0;
}

void File::display(std::ostream& os) const
{
	os << m_name << std::endl;
}

// ---------- Directory ------------
Directory::Directory(const std::string& name)
	: File(name) { }

std::shared_ptr<const Directory> Directory::create(const std::string& path)
{
	std::shared_ptr<Directory> directory(new Directory(path));
	
	DirGuard dir(path);
	struct dirent* entry;
	
	while((entry = readdir(dir.get())))
	{
		std::string temp_path = path;
		temp_path.append("/").append(entry->d_name);
		
		DirGuard is_dir(temp_path);
		if(strcmp(".", entry->d_name) && strcmp(entry->d_name, "..") && 
															is_dir.get())
		{
			directory->add(create(temp_path));
		}
		else
		{
			directory->add(std::shared_ptr<File>(new File(temp_path)));
		}
	}
	
	return directory;
}


std::shared_ptr<const File> 
						Directory::find(const std::string& name) const noexcept 
{
	FindFunctor find_func(name);
	find_func = for_each(m_files.begin(), m_files.end(), find_func);
	
	return find_func.get();
}

void Directory::display(std::ostream& os) const
{
	DisplayFunctor disp(os);
	for_each(m_files.begin(), m_files.end(), disp);
}

void Directory::add(std::shared_ptr<const File> entry)
{
	m_files.push_back(entry);
}

} // namespace ilrd

using namespace ilrd;

int main()
{
	std::shared_ptr<const Directory> dir = Directory::create("dir");
	std::shared_ptr<const File> found = dir->find("file.txt");
	
	dir->display(std::cout);
	std::cout << "---" << std::endl;
	found->display(std::cout);
	
	return 0;
}
