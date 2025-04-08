#pragma once

#include "http/HTTPParser.hpp"
#include "http/HTTPRequest.hpp"
#include "networking/Connection.hpp"

class HTTPSConnection : public Connection
{
public:
	HTTPSConnection(int socketfd);
	~HTTPSConnection();

	bool handle() override;
	Request getRequest() const override;

private:
	HTTPParser parser;
	HTTPRequest request;
};
