#include "cryptography/ecc/WeierstrassCurve.hpp"

Point WeierstrassCurve::scalarMultiply(const Point& G, const Number& n) const
{
	std::string bits = n.toBinaryString();

	Point R0 = Point::Identity;
	Point R1 = G;

	for (const auto& bit : bits)
	{
		if (bit == '0')
		{
			R1 = addPoints(R0, R1);
			R0 = doublePoint(R0);
		}
		else if (bit == '1')
		{
			R0 = addPoints(R0, R1);
			R1 = doublePoint(R1);
		}
	}

	return R0;
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
