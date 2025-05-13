#pragma once

#include "cryptography/ecc/Point.hpp"
#include "math/Number.hpp"

class EllipticCurve
{
public:
	explicit EllipticCurve(const Number& a, const Number& b, const Number& p);
	~EllipticCurve();

	EllipticCurve(const EllipticCurve&);
	EllipticCurve& operator=(const EllipticCurve&);

	EllipticCurve(EllipticCurve&&) noexcept;
	EllipticCurve& operator=(EllipticCurve&&) noexcept;

	[[nodiscard]] virtual Point multiply(const Point& point, const Number& scalar) const = 0;

	[[nodiscard]] virtual Point addPoints(const Point& p1, const Point& p2) const = 0;

	[[nodiscard]] virtual Point doublePoint(const Point& point) const = 0;

	[[nodiscard]] virtual bool isOnCurve(const Point& point) const = 0;

protected:
	Number a;

	Number b;

	Number p;
};
