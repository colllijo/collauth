#pragma once

#include "math/Number.hpp"

class MontgomeryCurve
{
public:
	explicit MontgomeryCurve(const Number& A, const Number& B, const Number& p);

	Number multiply(const Number& generator, const Number& scalar) const;

private:
	Number A;
	Number B;

	Number p;

	struct Point
	{
		Number X;
		Number Z;
	};

	Point addPoints(const Point& p1, const Point& p2) const;

	Point doublePoint(const Point& point) const;
};
