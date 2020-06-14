#include <iostream> // cout
#include <cstdlib> // rand
#include <fstream> // ofstream
 
#include "surprizer.h"

typedef Surprizer* (*func_ptr_t)(); 

Surprizer* Surprizer::GetRandomSurprize()
{
	srand(time(NULL));	
	const int num_of_surprizes = 3;
	func_ptr_t arr[num_of_surprizes] = { PrintSurprize, EchoSurprize, 
										FileSurprize };
	
	return arr[rand() % num_of_surprizes]();
}

void Surprizer::SurprizeMe()
{
	std::cout << "Surprize!" << std::endl;
}

void File::SurprizeMe()
{
	std::cout << "Making file" << std::endl;
	std::ofstream file("nothing_here.txt");	
}

void Echo::SurprizeMe()
{
	system("echo '\\a'");	
}

Surprizer* EchoSurprize()
{
	return new Echo;
}

Surprizer* FileSurprize()
{
	return new File;
}

Surprizer* PrintSurprize()
{
	return new Surprizer;
}

int main() 
{

	Surprizer* today_surprize = Surprizer::GetRandomSurprize();
	
	today_surprize->SurprizeMe();
	
	delete today_surprize;
	
	return 0;
}
