#pragma once

#include "cryptography/ecc/Point.hpp"
#include "math/Number.hpp"

/**
 * @class EllipticCurve
 * @brief Abstract base class for elliptic curve cryptography.
 *
 * This class defines the interface for elliptic curves over a finite field,
 * supporting both Weierstrass and Montgomery forms. The parameters \( a \) and \( b \)
 * represent the curve parameters for either the Weierstrass form:
 * @f[
 *     y^2 = x^3 + ax + b \pmod{p}
 * @f]
 * or the Montgomery form:
 * @f[
 *     By^2 = x^3 + Ax^2 + x \pmod{p}
 * @f]
 * where \( a \), \( b \) (or \( A \), \( B \)) are curve parameters and \( p \) is the prime modulus.
 */
class EllipticCurve
{
public:
    /**
     * @brief Constructs an elliptic curve with given parameters.
     * @param a The curve parameter (\( a \) for Weierstrass, \( A \) for Montgomery).
     * @param b The curve parameter (\( b \) for Weierstrass, \( B \) for Montgomery).
     * @param p The prime modulus \( p \) of the finite field.
     *
     * The curve is defined by either the Weierstrass or Montgomery equation.
     */
	explicit EllipticCurve(const Number& a, const Number& b, const Number& p);

	/**
	 * @brief Destructor.
	 */
	~EllipticCurve();

    /// Deleted copy constructor.
	EllipticCurve(const EllipticCurve&) = delete;

    /// Deleted copy assignment operator.
	EllipticCurve& operator=(const EllipticCurve&) = delete;

    /// Deleted move constructor.
	EllipticCurve(EllipticCurve&&) noexcept = delete;

    /// Deleted move assignment operator.
	EllipticCurve& operator=(EllipticCurve&&) noexcept = delete;

    /**
     * @brief Multiplies a point by a scalar on the curve.
     * @param point The point to multiply.
     * @param scalar The scalar value.
     * @return The resulting point after multiplication.
     *
     * Computes \( Q = kP \), where \( P \) is a point on the curve and \( k \) is a scalar.
     */
	[[nodiscard]] virtual Point multiply(const Point& point, const Number& scalar) const = 0;

    /**
     * @brief Adds two points on the curve.
     * @param p1 The first point.
     * @param p2 The second point.
     * @return The resulting point after addition.
     *
     * Computes \( R = P + Q \), where \( P \) and \( Q \) are points on the curve.
     */
	[[nodiscard]] virtual Point addPoints(const Point& p1, const Point& p2) const = 0;

    /**
     * @brief Doubles a point on the curve.
     * @param point The point to double.
     * @return The resulting point after doubling.
     *
     * Computes \( Q = 2P \), where \( P \) is a point on the curve.
     */
	[[nodiscard]] virtual Point doublePoint(const Point& point) const = 0;

    /**
     * @brief Checks if a point lies on the curve.
     * @param point The point to check.
     * @return True if the point is on the curve, false otherwise.
     *
     * Verifies if the given point satisfies the curve equation.
     */
	[[nodiscard]] virtual bool isOnCurve(const Point& point) const = 0;

protected:
	Number a; ///< Curve parameter (\( a \) for Weierstrass, \( A \) for Montgomery).
	Number b; ///< Curve parameter (\( b \) for Weierstrass, \( B \) for Montgomery).
	Number p; ///< Prime modulus \( p \) of the finite field.
};
