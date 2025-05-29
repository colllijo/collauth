#pragma once

#include "cryptography/ecc/Point.hpp"
#include "math/Number.hpp"
class WeierstrassCurve
{
public:
	explicit WeierstrassCurve(const Number& a, const Number& b, const Number& p);

	Point scalarMultiply(const Point& G, const Number& n) const;

private:
	Number a;
	Number b;

	Number p;

	Point addPoints(const Point& P, const Point& Q) const;
	Point doublePoint(const Point& P) const;
};
