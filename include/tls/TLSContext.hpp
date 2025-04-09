#pragma once

#include <cstdint>
#include <vector>

class TLSContext
{
public:
	enum class TLSState
	{
		INITIAL,
		NEGOTIATING,
		HANDSHAKE_COMPLETE,
		APPLICATION_DATA,
		CLOSED
	};

	TLSContext();

	bool initialize(const std::vector<uint8_t>& data);

	bool ready() const;

private:
	TLSState state;

	// CipherSuite selectedCipherSuite
};
