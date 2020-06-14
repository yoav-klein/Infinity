
#include <iostream>
class Shape
{
public:
enum ShapeColor { Red, Green };

virtual void Draw(ShapeColor color = Green)
{
	std::cout << color;
}

};

class Rectangle : public Shape
{

virtual void Draw(ShapeColor color)
{
	std::cout << color;
}

};

int main()
{
	int arr[10][30][2];
	arr[0][2][3] = 2;
	//Shape* rs = new Rectangle;
	//rs->Draw();
	//rs->Draw(Shape::Red);
	
	return 0;
}
