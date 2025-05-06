#pragma once

#include "math/Number.hpp"

class FiniteField
{
public:
	FiniteField(const Number& modulus);

	Number add(const Number& a, const Number& b) const;
	Number sub(const Number& a, const Number& b) const;
	Number mul(const Number& a, const Number& b) const;
	Number div(const Number& a, const Number& b) const;

private:
	Number modulus;

	Number inverse(const Number& value) const;
};
