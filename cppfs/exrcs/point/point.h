
struct Point
{
	enum paranthesis
	{
		ROUND,
		SQUARE,
		CURLY,
		ANGULAR,
		NUM_OF_PARANS
	};
	
	int m_x;
	int m_y;
	static float g_total_length;
	
	explicit Point(int x = 0, int y = 0);
	Point(const Point& other);
	Point& operator=(const Point& _other);
	~Point();
	
	void UpdateLength();
	bool IsEqual(const Point pnt) const;
	Point& AdjustBy(const Point &pnt);
	Point& AdjustBy(int x, int y);
	float Length() const;
	void Print(char open, char close) const;	
	void Print(enum paranthesis paran = ANGULAR) const;
	float length;
};

struct Point Add(struct Point pnt_a, struct Point pnt_b);

float GetTotalLength();
