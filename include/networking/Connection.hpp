#pragma once

#include <variant>

#include "http/HTTPRequest.hpp"
#include "networking/Socket.hpp"

using Request = std::variant<HTTPRequest>;

class Connection
{
public:
	explicit Connection(int socketfd);
	virtual ~Connection() = default;

	virtual bool handle() = 0;

	bool isRequestReady() const;
	virtual Request getRequest() const = 0;

	int getSocketFd() const;

protected:
	Socket socket;
	bool requestReady;
};
