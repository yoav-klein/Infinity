#include <iostream>
#include <cstdlib>

struct X
{
	int m_x;
};

struct Y
{
	int m_y;
};

int main() 
{
   X* x1 = new X;
   free(x1);
   
   X* x2 = (X*)malloc(sizeof(*x2));
   delete x2;
   
   return 0;
}
