#include "cryptography/ecc/Point.hpp"

const Point Point::Zero = Point(0, 0);
const Point Point::Identity = Point();

Point::Point() : x(0), y(0), infinity(true) {}
Point::Point(const Number& x, const Number& y) : x(x), y(y), infinity(false) {}

bool Point::isInfinity() const
{
	return infinity;
}

bool Point::operator==(const Point& other) const
{
	if (infinity && other.infinity) return true;
	if (infinity || other.infinity) return false;

	return x == other.x && y == other.y;
}

Point Point::operator-() const
{
	if (infinity) return *this;
	return Point(x, -y);
}
