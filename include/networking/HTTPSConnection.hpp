#pragma once

#include "http/HTTPParser.hpp"
#include "http/HTTPRequest.hpp"
#include "networking/Connection.hpp"
#include "tls/TLSContext.hpp"

class HTTPSConnection : public Connection
{
public:
	HTTPSConnection(int socketfd);
	~HTTPSConnection();

	bool handle() override;
	Request getRequest() const override;

private:
	TLSContext context;

	HTTPParser parser;
	HTTPRequest request;
};
