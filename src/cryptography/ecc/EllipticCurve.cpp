#include "cryptography/ecc/EllipticCurve.hpp"

EllipticCurve::EllipticCurve(const Number& a, const Number& b, const Number& p) : a(a), b(b), p(p) {}

EllipticCurve::~EllipticCurve() = default;

EllipticCurve::EllipticCurve(const EllipticCurve& other) = default;

EllipticCurve& EllipticCurve::operator=(const EllipticCurve& other) = default;

EllipticCurve::EllipticCurve(EllipticCurve&& other) noexcept = default;

EllipticCurve& EllipticCurve::operator=(EllipticCurve&& other) noexcept = default;
