#pragma once

class Rectangle
{
public:
	int X1;
	int Y1;
	int X2;
	int Y2;
public:
	Rectangle(int x, int y, int w, int h)
		: X1(x), Y1(y), X2(x + w), Y2(y + h)
	{
	}

	void GetCenter(int& x, int& y) const
	{
		x = int((float(X1 + X2) / 2.0f));
		y = int((float(Y1 + Y2) / 2.0f));
	}
	
	bool Intersects(const Rectangle& other)
	{
		return (X1 <= other.X2 && X2 >= other.X1 && Y1 >= other.Y2 && Y2 <= other.Y1);
	}
};
