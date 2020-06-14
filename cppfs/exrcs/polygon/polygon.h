#include <stddef.h>
#include "../point/point.h"

struct Polygon
{
	explicit Polygon(size_t num_of_points = 0);
	~Polygon();
	
	Point *m_points;
	size_t m_index;
	size_t m_max_points;
	
	int Add(int x, int y);
	void Print();
	bool IsEqual(const Polygon& P);
};
