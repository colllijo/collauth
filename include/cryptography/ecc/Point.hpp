#pragma once

#include "math/Number.hpp"

struct Point
{
	Number x;
	Number y;
	Number z;

	bool infinity;

	Point();
	Point(const Number& x, const Number& y);
	Point(const Number& x, const Number& y, const Number& z);

	bool operator==(const Point& other) const;

	Point operator-() const;

	bool isInfinity() const;

	const static Point Zero;
	const static Point Identity;
};
