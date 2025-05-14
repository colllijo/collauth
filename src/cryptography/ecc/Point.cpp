/**
 * @file Point.cpp
 * @brief Implementation of the Point struct for elliptic curve points.
 */

#include "cryptography/ecc/Point.hpp"

const Point Point::Zero = Point(0, 0);
const Point Point::Identity = Point(0, 0, true);

/**
 * @brief Checks if this point is the point at infinity.
 * @return True if the point is at infinity, false otherwise.
 */
bool Point::isInfinite() const
{
	return infinite;
}

/**
 * @brief Equality operator for Point.
 * @param other The point to compare with.
 * @return True if the points are equal, false otherwise.
 */
bool Point::operator==(const Point& other) const
{
	if (infinite && other.infinite)
	{
		return true;
	}
	if (infinite || other.infinite)
	{
		return false;
	}

	return x == other.x && y == other.y;
}
