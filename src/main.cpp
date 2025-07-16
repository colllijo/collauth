#include "cryptography/Random.hpp"
#include "math/Number.hpp"

int main()
{
	// FFDHE2048 parameters from RFC 7919
	Number ffdhe2048_prime = Number(
		"FFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695A9E13641146433FBCC939DCE249B3EF97D2FE363630C75D8F681B202AE4C8F6D7EBF48E1D8E6F9A47A82E56496FDEE372FE9CFEE5E"
		"CFCF6F1FE3B1B4C6FAD73CB6A6B5B43F8F4F302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A63A36210000000000090563",
		16);
	Number g = 2;  // Generator for FFDHE groups

	// Each party chooses a private key
	Number a = generateRandomNumber(ffdhe2048_prime - 2) + 2;
	Number b = generateRandomNumber(ffdhe2048_prime - 2) + 2;

	// Compute public keys
	Number A = Number::modPow(g, a, ffdhe2048_prime);
	Number B = Number::modPow(g, b, ffdhe2048_prime);

	// Exchange and compute shared secrets
	Number shared_secret_a = Number::modPow(B, a, ffdhe2048_prime);
	Number shared_secret_b = Number::modPow(A, b, ffdhe2048_prime);

	return 0;
}
