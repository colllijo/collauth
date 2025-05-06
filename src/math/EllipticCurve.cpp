#include "math/EllipticCurve.hpp"

#include "math/FiniteField.hpp"
#include "math/Point.hpp"

EllipticCurve::EllipticCurve() : a(0), b(0), p(0), field(0) {}
EllipticCurve::EllipticCurve(const Number& a, const Number& b, const Number& p) : a(a), b(b), p(p), field(p) {}

Point EllipticCurve::multiply(const Point& p, const Number& scalar) const
{
	Number k = scalar;
	Point result = Point::Identity;
	Point addend = p;

	while (k > 0)
	{
		if (k % 2 == 1)
		{
			result = addPoints(result, addend);
		}

		addend = doublePoint(addend);
		k >>= 1;
	}

	return result;
}

bool EllipticCurve::isOnCurve(const Point& point) const
{
	FieldElement left = point.y * point.y;
	FieldElement right = (point.x * point.x * point.x) + (a * point.x) + b;

	return left == right;
}

Point EllipticCurve::addPoints(const Point& p1, const Point& p2) const
{
	if (p1 == Point::Identity) return p2;
	if (p2 == Point::Identity) return p1;

	if (p1 == p2) return doublePoint(p1);

	FieldElement slope = (p2.y - p1.y) / (p2.x - p1.x);
	FieldElement x = (slope * slope) - p1.x - p2.x;
	FieldElement y = slope * (p1.x - x) - p1.y;

	return Point(x, y);
}

Point EllipticCurve::doublePoint(const Point& p) const
{
	const FieldElement two = FieldElement(2, EllipticCurve::p);
	const FieldElement three = FieldElement(3, EllipticCurve::p);

	FieldElement slope = (three * (p.x * p.x) + a) / two * p.y;
	FieldElement x = (slope * slope) - (two * p.x);
	FieldElement y = slope * (p.x - x) - p.y;

	return Point(x, y);
}
