


template <typename T, typename... Types>
class Variadic
{
public:

private:
	T m_var;
};


int main()
{
	Variadic<int, int, float> variadic1;
	Variadic<int> variadic2;
}
