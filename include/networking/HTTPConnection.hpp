#pragma once

#include "http/HTTPParser.hpp"
#include "http/HTTPRequest.hpp"
#include "networking/Connection.hpp"

class HTTPConnection : public Connection
{
public:
	explicit HTTPConnection(int socketfd);
	~HTTPConnection() override;

	bool handle() override;
	Request getRequest() const override;

private:
	HTTPParser parser;
	HTTPRequest request;
};
