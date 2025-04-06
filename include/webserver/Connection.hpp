#pragma once

#include "http/HttpParser.hpp"
#include "http/HttpRequest.hpp"
#include "networking/Socket.hpp"

class Connection
{
public:
	Connection(int socketfd);
	~Connection();

	bool handleRead();
	bool isRequestReady() const;

	HttpRequest getRequest() const;

	int getSocketFd() const;

private:
	Socket socket;
	HttpParser parser;
	bool requestReady;
};
