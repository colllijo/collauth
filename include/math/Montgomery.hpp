#pragma once

#include "math/Number.hpp"

namespace montgomery
{
	Number multiply(const Number& a, const Number& b);

	Number redc(const Number& R, const Number& N, const Number& N_Prime, const Number& T);

	Number modPow(const Number& base, const Number& exponent, const Number& modulus);

	Number toMontgomeryForm(const Number& x, const Number& modulus);
};	// namespace montgomery
