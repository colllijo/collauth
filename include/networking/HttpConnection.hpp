#pragma once

#include "http/HttpParser.hpp"
#include "http/HttpRequest.hpp"
#include "networking/Connection.hpp"

class HttpConnection : public Connection
{
public:
	HttpConnection(int socketfd);
	~HttpConnection();

	bool handle() override;
	Request getRequest() const override;

private:
	HttpParser parser;
	HttpRequest request;
};
