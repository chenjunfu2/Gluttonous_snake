#pragma once

struct My_Point
{
	long x, y;

public:
	My_Point &operator+=(const My_Point &_Other)
	{
		x += _Other.x;
		y += _Other.y;
		return *this;
	}
};
