#include "cryptography/ecc/MontgomeryCurve.hpp"

#include "cryptography/ecc/Point.hpp"
#include "logging/Logger.hpp"
#include "math/Montgomery.hpp"

MontgomeryCurve::MontgomeryCurve(const Number& A, const Number& B, const Number& p) : A(A), B(B), p(p) {}

Point MontgomeryCurve::scalarMultiply(const Point& G, const Number& n) const
{
	std::string bits = n.toBinary();

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

	Number X = (D.z * ((P.x - P.z) * (Q.x + Q.z) + (P.x + P.z) * (Q.x - Q.z)).pow(2)) % p;
	Number Z = (D.x * ((P.x - P.z) * (Q.x + Q.z) - (P.x + P.z) * (Q.x - Q.z)).pow(2)) % p;

	Logger::info("({}, {}) + ({}, {}) = ({}, {})", P.x, P.z, Q.x, Q.z, X, Z);
	return Point(X, 0, Z);
}

Point MontgomeryCurve::doublePoint(const Point& P) const
{
	Number FOUR_X_ONE_Z = ((P.x + P.z).pow(2) - (P.x - P.z).pow(2)) % p;

	Number X = ((P.x + P.z).pow(2) * (P.x - P.z).pow(2)) % p;
	Number Z = (FOUR_X_ONE_Z * ((P.x - P.z).pow(2) + ((A + 2) * montgomery::modPow(4, p - 2, p)) * (FOUR_X_ONE_Z))) % p;

	Logger::info("2 * ({}, {}) = ({}, {})", P.x, P.z, X, Z);
	return Point(X, 0, Z);
}
