#pragma once

struct My_Point
{
public:
	long x, y;
public:
	My_Point &operator+=(const My_Point &_Right)
	{
		x += _Right.x;
		y += _Right.y;
		return *this;
	}

	My_Point &operator-=(const My_Point &_Right)
	{
		x -= _Right.x;
		y -= _Right.y;
		return *this;
	}

	My_Point operator+(const My_Point &_Right) const
	{
		return My_Point
		{
			x + _Right.x,
			y + _Right.y,
		};
	}

	My_Point operator-(const My_Point &_Right) const
	{
		return My_Point
		{
			x - _Right.x,
			y - _Right.y,
		};
	}
};
