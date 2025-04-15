#pragma once

#include <cstdint>
#include <vector>

enum class TLSSignatureScheme : uint16_t
{
	/* RSASSA-PKCS1-v1_5 algorithms */
	RSA_PKCS1_SHA256 = 0x0401,
	RSA_PKCS1_SHA384 = 0x0501,
	RSA_PKCS1_SHA512 = 0x0601,

	/* ECDSA algorithms */
	ECDSA_SECP256R1_SHA256 = 0x0403,
	ECDSA_SECP384R1_SHA384 = 0x0503,
	ECDSA_SECP521R1_SHA512 = 0x0603,

	/* RSASSA-PSS algorithms with public key OID rsaEncryption */
	RSA_PSS_RSAE_SHA256 = 0x0804,
	RSA_PSS_RSAE_SHA384 = 0x0805,
	RSA_PSS_RSAE_SHA512 = 0x0806,

	/* EdDSA algorithms */
	ED25519 = 0x0807,
	ED448 = 0x0808,

	/* RSASSA-PSS algorithms with public key OID RSASSA-PSS */
	RSA_PSS_PSS_SHA256 = 0x0809,
	RSA_PSS_PSS_SHA384 = 0x080a,
	RSA_PSS_PSS_SHA512 = 0x080b,

	/* Legacy algorithms */
	RSA_PKCS1_SHA1 = 0x0201,
	ECDSA_SHA1 = 0x0203
};

std::vector<TLSSignatureScheme> parseTLSSignatureSchemes(const std::vector<uint8_t>& data);
