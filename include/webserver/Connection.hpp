#pragma once

#include "http/HttpParser.hpp"
#include "http/HttpRequest.hpp"
#include "networking/Socket.hpp"

class Connection
{
public:
	Connection(int socketfd, bool tls);
	~Connection();

	bool handleRead();
	bool isRequestReady() const;

	HttpRequest getRequest() const;

	int getSocketFd() const;

private:
	Socket socket;
	bool isTls;

	HttpParser parser;
	bool requestReady;
};
