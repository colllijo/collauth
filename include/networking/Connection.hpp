#pragma once

#include <variant>

#include "http/HttpRequest.hpp"
#include "networking/Socket.hpp"

using Request = std::variant<HttpRequest>;

class Connection
{
public:
	Connection(int socketfd);
	virtual ~Connection() = default;

	virtual bool handle() = 0;

	bool isRequestReady() const;
	virtual Request getRequest() const = 0;

	int getSocketFd() const;

protected:
	Socket socket;
	bool requestReady;
};
