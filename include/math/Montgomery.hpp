#pragma once

#include "math/Number.hpp"

namespace montgomery
{
	struct MontgomeryContext
	{
		Number N;		// Modulus (Must be odd)
		Number R;		// power of 2 ≥ N (R = 2^k)
		Number Rmask;	// R-1 (for fast mod R via bit-and)
		Number Nprime;	// -N^-1 (mod R)
		size_t k;		// k = log2(R)

		explicit MontgomeryContext(const Number& modulus);

		Number toMontgomery(const Number& a) const;
		Number fromMontgomery(const Number& a) const;

		Number reduce(const Number& T) const;

		Number multiply(const Number& a, const Number& b) const;
	};

	Number modPow(const Number& base, const Number& exponent, const Number& modulus);
};	// namespace montgomery
