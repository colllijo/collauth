#pragma once

#include <cstdint>
#include <memory>

#include "networking/Connection.hpp"

enum class Protocol : uint8_t
{
	HTTP,
	HTTPS
};

class ConnectionFactory
{
public:
	static std::unique_ptr<Connection> createConnection(int fd, Protocol protocol);
};
