#pragma once

#include "cryptography/ecc/Point.hpp"
#include "math/Number.hpp"

class MontgomeryCurve
{
public:
	explicit MontgomeryCurve(const Number& A, const Number& B, const Number& p);

	Point scalarMultiply(const Point& G, const Number& n) const;

private:
	Number A;
	Number B;

	Number p;

	Point addPoints(const Point& P, const Point& Q, const Point& D) const;

	Point doublePoint(const Point& point) const;
};
