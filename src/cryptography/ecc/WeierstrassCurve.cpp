#include "cryptography/ecc/WeierstrassCurve.hpp"

WeierstrassCurve::WeierstrassCurve(const Number& a, const Number& b, const Number& p) : EllipticCurve(a, b, p) {}

Point WeierstrassCurve::multiply(const Point& p, const Number& scalar) const
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

bool WeierstrassCurve::isOnCurve(const Point& point) const
{
	Number left = point.y.pow(2);
	Number right = (point.x.pow(3)) + (a * point.x) + b;

	return (left - right) % p == 0;
}

Point WeierstrassCurve::addPoints(const Point& p1, const Point& p2) const
{
	if (p1 == Point::Identity)
	{
		return p2;
	}
	if (p2 == Point::Identity)
	{
		return p1;
	}

	if (p1 == p2)
	{
		return doublePoint(p1);
	}

	Number slope = ((p2.y - p1.y) * (p2.x - p1.x).modInverse(p)) % p;
	Number x = (slope.pow(2) - p1.x - p2.x) % p;
	Number y = (slope * (p1.x - x) - p1.y) % p;

	return Point(x, y);
}

Point WeierstrassCurve::doublePoint(const Point& point) const
{
	if (point == Point::Identity)
	{
		return point;
	}

	Number slope = ((static_cast<Number>(3) * point.x.pow(2) + a) * (static_cast<Number>(2) * point.y).modInverse(p)) % p;
	Number x = (slope.pow(2) - static_cast<Number>(2) * point.x) % p;
	Number y = (slope * (point.x - x) - point.y) % p;

	return Point(x, y);
}
