#pragma once

#include "cryptography/ecc/EllipticCurve.hpp"
#include "math/Number.hpp"

class WeierstrassCurve : public EllipticCurve
{
public:
	explicit WeierstrassCurve(const Number& a, const Number& b, const Number& p);

	Point multiply(const Point& point, const Number& scalar) const override;

	Point addPoints(const Point& p1, const Point& p2) const override;

	Point doublePoint(const Point& point) const override;

	bool isOnCurve(const Point& point) const override;
};
