#include "math/Montgomery.hpp"

#include <cassert>

#include "logging/Logger.hpp"

namespace montgomery
{
	MontgomeryContext::MontgomeryContext(const Number& modulus) : N(modulus), R(1), Rmask(0), Nprime(0), k(0)
	{
		if (N.isEven()) throw std::invalid_argument("Modulus must be odd");

		k = ((N.bits() + Number::BASE - 1) / Number::BASE) * Number::BASE;
		R <<= k;
		Rmask = R - 1;

		auto tuple = Number::extendedGCD(N, R);
		Logger::debug("eGCD = ({}, {}, {})", std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));

		auto [g, x, y] = Number::extendedGCD(N, R);
		Logger::debug("{} (expected {})", (N * x + R * y), g);

		Nprime = (R - Number::modInverse(N, R)) & Rmask;
	}

	Number MontgomeryContext::toMontgomery(const Number& a) const
	{
		return (a * R) % N;
	}

	Number MontgomeryContext::fromMontgomery(const Number& a) const
	{
		return reduce(a);
	}

	Number MontgomeryContext::reduce(const Number& T) const
	{
		Number m = ((T & Rmask) * Nprime) & Rmask;
		Number t = (T + m * N) >> k;

		if (t >= N) return t - N;
		return t;
	}

	Number MontgomeryContext::multiply(const Number& a, const Number& b) const
	{
		return reduce(a * b);
	}

	Number modPow(const Number& base, const Number& exponent, const Number& modulus)
	{
		if (modulus == 1) return 0;
		if (exponent == 0) return 1 % modulus;

		MontgomeryContext context(modulus);

		Number b = context.toMontgomery(base % modulus);
		Number accumulator = context.toMontgomery(1);

		for (size_t i = exponent.bits(); i-- > 0;)
		{
			Logger::debug("Iteration {}/{}", exponent.bits() - i, exponent.bits());
			accumulator = context.multiply(accumulator, accumulator);
			if ((exponent >> i) & 1)
			{
				accumulator = context.multiply(accumulator, b);
			}
		}

		return context.fromMontgomery(accumulator);
	}
}  // namespace montgomery
