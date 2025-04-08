#pragma once

#include "http/HTTPParser.hpp"
#include "http/HTTPRequest.hpp"
#include "networking/Connection.hpp"

class HTTPConnection : public Connection
{
public:
	HTTPConnection(int socketfd);
	~HTTPConnection();

	bool handle() override;
	Request getRequest() const override;

private:
	HTTPParser parser;
	HTTPRequest request;
};
