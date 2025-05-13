#pragma once

#include "http/HTTPParser.hpp"
#include "http/HTTPRequest.hpp"
#include "networking/Connection.hpp"
#include "tls/TLSContext.hpp"

class HTTPSConnection : public Connection
{
public:
	explicit HTTPSConnection(int socketfd);
	~HTTPSConnection() override;

	bool handle() override;
	Request getRequest() const override;

private:
	TLSContext context;

	HTTPParser parser;
	HTTPRequest request;
};
