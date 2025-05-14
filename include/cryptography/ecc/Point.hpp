#pragma once

#include <utility>

#include "math/Number.hpp"

/**
 * @struct Point
 * @brief Represents a point on an elliptic curve.
 *
 * This struct models a point \((x, y)\) on an elliptic curve over a finite field.
 * It also supports the concept of the point at infinity, which acts as the identity element
 * in elliptic curve group operations.
 */
struct Point
{
    Number x;      ///< The x-coordinate of the point.
    Number y;      ///< The y-coordinate of the point.
    bool infinite; ///< True if this point is the point at infinity (identity element).

    /**
     * @brief Constructs a finite point with given coordinates.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     */
	Point(Number x, Number y) : x(std::move(x)), y(std::move(y)), infinite(false) {}

    /**
     * @brief Constructs a point with given coordinates and infinity flag.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     * @param infinite True if this point is the point at infinity.
     */
	Point(Number x, Number y, bool infinite) : x(std::move(x)), y(std::move(y)), infinite(infinite) {}

    /**
     * @brief Checks if this point is the point at infinity.
     * @return True if the point is at infinity, false otherwise.
     */
	bool isInfinite() const;

    /**
     * @brief Equality operator.
     * @param other The point to compare with.
     * @return True if the points are equal, false otherwise.
     */
	bool operator==(const Point& other) const;

    static const Point Zero;     ///< The point at (0, 0).
    static const Point Identity; ///< The point at infinity (identity element).
};
