#pragma once

#include <cstdint>
#include <vector>

#include "networking/Socket.hpp"

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

	explicit TLSContext(Socket& socket);

	bool initialize(const std::vector<uint8_t>& data);

	bool ready() const;

private:
	Socket& socket;
	TLSState state;

	bool exchangeHandshake(const std::vector<uint8_t>& data);
	// CipherSuite selectedCipherSuite
};
