#pragma once

#include "math/Number.hpp"

struct Point
{
	Number x;
	Number y;

	bool infinity;

	Point();
	Point(const Number& x, const Number& y);

	bool operator==(const Point& other) const;

	Point operator-() const;

	bool isInfinity() const;

	const static Point Zero;
	const static Point Identity;
};
