#include "math/FiniteField.hpp"

FiniteField::FiniteField(const Number& modulus) : modulus(modulus) {}

Number FiniteField::add(const Number& a, const Number& b) const
{
	return (a + b) % modulus;
}

Number FiniteField::sub(const Number& a, const Number& b) const
{
	return (a - b + modulus) % modulus;
}

Number FiniteField::mul(const Number& a, const Number& b) const
{
	return (a * b) % modulus;
}

Number FiniteField::div(const Number& a, const Number& b) const
{
	return a * inverse(b);
}

Number FiniteField::inverse(const Number& value) const
{
	return Number::modInverse(value, modulus), modulus;
}
