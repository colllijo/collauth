#pragma once

#include <memory>

#include "networking/Connection.hpp"

enum class Protocol
{
	HTTP,
	HTTPS
};

class ConnectionFactory
{
public:
	static std::unique_ptr<Connection> createConnection(int fd, Protocol protocol);
};
