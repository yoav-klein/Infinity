

template<typename T, typename... Types>
class Factory
{
public:
	Factory() { }
	
	void Create(T v1, Types... vars);

private:

};

template<typename T, typename... Types>
void Factory<T, Types...>::Create(T v1, Types... vars)
{
	std::cout << v1 << std::endl;
}

template<typename... Types>
void Display(Types... vars)
{
	std::cout << vars[0] <<std::endl;
	
	//Display(vars...);
}
