#include "cryptography/ecc/WeierstrassCurve.hpp"

#include <algorithm>

WeierstrassCurve::WeierstrassCurve(const Number& a, const Number& b, const Number& p) : a(a), b(b), p(p) {}

Point WeierstrassCurve::scalarMultiply(const Point& G, const Number& n) const
{
	std::string bits = n.toBinaryString();
	std::reverse(bits.begin(), bits.end());

	Point result = Point::Identity;
	Point accumulator = G;

	for (const auto& bit : bits)
	{
		if (bit == '1') result = addPoints(result, accumulator);
		accumulator = doublePoint(accumulator);
	}

	return result;
}

Point WeierstrassCurve::addPoints(const Point& P, const Point& Q) const
{
	if (P.isInfinity()) return Q;
	if (Q.isInfinity()) return P;
	if (P == -Q) return Point::Identity;

	if (P == Q) return doublePoint(P);

	const Number slope = (((P.y - Q.y) % p) * ((P.x - Q.x) % p).modInverse(p)) % p;

	const Number x = ((slope * slope) - P.x - Q.x) % p;
	const Number y = (-P.y + slope * (P.x - x)) % p;

	return Point(x, y);
}

Point WeierstrassCurve::doublePoint(const Point& P) const
{
	const Number slope = (((3 * (P.x * P.x) + a) % p) * ((2 * P.y) % p).modInverse(p)) % p;

	const Number x = ((slope * slope) - P.x - P.x) % p;
	const Number y = (-P.y + slope * (P.x - x)) % p;

	return Point(x, y);
}
