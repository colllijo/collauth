/**
 * @file EllipticCurve.cpp
 * @brief Implementation of the EllipticCurve base class for elliptic curve cryptography.
 */

#include "cryptography/ecc/EllipticCurve.hpp"

/**
 * @brief Constructs an elliptic curve with the given parameters.
 * @param a The curve parameter (\( a \) for Weierstrass, \( A \) for Montgomery).
 * @param b The curve parameter (\( b \) for Weierstrass, \( B \) for Montgomery).
 * @param p The prime modulus \( p \) of the finite field.
 */
EllipticCurve::EllipticCurve(const Number& a, const Number& b, const Number& p) : a(a), b(b), p(p) {}

/**
 * @brief Destructor for EllipticCurve.
 */
EllipticCurve::~EllipticCurve() = default;
