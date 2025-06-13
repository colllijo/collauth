#include "cryptography/ecc/MontgomeryCurve.hpp"

#include "cryptography/ecc/Point.hpp"

MontgomeryCurve::MontgomeryCurve(const Number& A, const Number& B, const Number& p) : A(A), B(B), p(p) {}

Point MontgomeryCurve::scalarMultiply(const Point& G, const Number& n) const
{
	std::string bits = n.toBinaryString();

	Point R0 = Point::Identity;
	Point R1 = G;

	for (const auto& bit : bits)
	{
		if (bit == '0')
		{
			R1 = addPoints(R0, R1, G);
			R0 = doublePoint(R0);
		}
		else if (bit == '1')
		{
			R0 = addPoints(R0, R1, G);
			R1 = doublePoint(R1);
		}
	}

	return R0;
}

Point MontgomeryCurve::addPoints(const Point& P, const Point& Q, const Point& D) const
{
	if (P.isInfinity()) return Q;
	if (Q.isInfinity()) return P;
	if (P == -Q) return Point::Identity;

	if (P == Q) return doublePoint(P);

	Number A = ((P.x - P.z) * (Q.x + Q.z)) % p;
	Number B = ((P.x + P.z) * (Q.x - Q.z)) % p;

	Number x = (D.z * (A + B).pow(2)) % p;
	Number z = (D.x * (A - B).pow(2)) % p;

	return Point(x, 0, z);
}

Point MontgomeryCurve::doublePoint(const Point& P) const
{
	Number FOUR_X_Z = ((P.x + P.z).pow(2) - (P.x - P.z)) % p;

	Number x = ((P.x + P.z).pow(2) * (P.x - P.z).pow(2)) % p;
	Number z = (FOUR_X_Z * ((P.x - P.z).pow(2) + ((A + 2) * static_cast<Number>(4).modInverse(p)) * (FOUR_X_Z))) % p;

	return Point(x, 0, z);
}
