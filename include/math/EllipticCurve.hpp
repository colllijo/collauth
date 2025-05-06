#pragma once

#include "math/FiniteField.hpp"
#include "math/Number.hpp"
#include "math/Point.hpp"

/**
 * @class EllipticCurve
 * @brief Represents an elliptic curve defined by the equation \( y^2 = x^3 + ax + b \).
 *
 * This class provides the structure to define an elliptic curve over a field,
 * characterized by the coefficients \( a \) and \( b \).
 */
class EllipticCurve
{
public:
	/**
	 * @brief Default constructor for the EllipticCurve class.
	 *
	 * Initializes an elliptic curve with default coefficients \( a = 0 \) and \( b = 0 \).
	 */
	EllipticCurve();

	/**
	 * @brief Parameterized constructor for the EllipticCurve class.
	 *
	 * Initializes an elliptic curve with the given coefficients \( a \) and \( b \).
	 *
	 * @param a Coefficient \( a \) of the elliptic curve equation \( y^2 = x^3 + ax + b \).
	 * @param b Coefficient \( b \) of the elliptic curve equation \( y^2 = x^3 + ax + b \).
	 */
	EllipticCurve(const Number& a, const Number& b, const Number& p);

	/**
	 * @brief Multiplies a point on the elliptic curve by a scalar.
	 *
	 * This method performs scalar multiplication of a given point on the elliptic curve.
	 *
	 * @param point The point on the elliptic curve to be multiplied.
	 * @param scalar The scalar value by which the point is multiplied.
	 * @return The resulting point after scalar multiplication.
	 */
	Point multiply(const Point& point, const Number& scalar) const;

	/**
	 * @brief Adds two points on the elliptic curve.
	 *
	 * This method computes the sum of two points \( p1 \) and \( p2 \) on the elliptic curve.
	 *
	 * @param p1 The first point.
	 * @param p2 The second point.
	 * @return The resulting point after addition.
	 */
	Point addPoints(const Point& p1, const Point& p2) const;

	/**
	 * @brief Doubles a point on the elliptic curve.
	 *
	 * This method computes the result of doubling a given point on the elliptic curve.
	 *
	 * @param point The point to be doubled.
	 * @return The resulting point after doubling.
	 */
	Point doublePoint(const Point& point) const;

	/**
	 * @brief Checks if a given point lies on the elliptic curve.
	 *
	 * This method verifies whether the provided point satisfies the elliptic curve equation
	 * \( y^2 = x^3 + ax + b \).
	 *
	 * @param point The point to be checked.
	 * @return True if the point lies on the curve, false otherwise.
	 */
	bool isOnCurve(const Point& point) const;

private:
	/**
	 * @brief Coefficient \( a \) of the elliptic curve equation \( y^2 = x^3 + ax + b \).
	 */
	Number a;

	/**
	 * @brief Coefficient \( b \) of the elliptic curve equation \( y^2 = x^3 + ax + b \).
	 */
	Number b;

	Number p;

	FiniteField field;
};
