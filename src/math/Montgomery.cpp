#include "math/Montgomery.hpp"

Number montgomery::redc(const Number& R, const Number& N, const Number& N_Prime, const Number& T)
{
	const Number m = ((T % R) * N_Prime) % R;
	Number t = (T + m * N) / R;

	if (t >= N) return t - N;
	return t;
}
