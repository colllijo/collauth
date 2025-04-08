#pragma once

#include "http/HttpParser.hpp"
#include "http/HttpRequest.hpp"
#include "networking/Connection.hpp"

class HttpsConnection : public Connection
{
public:
	HttpsConnection(int socketfd);
	~HttpsConnection();

	bool handle() override;
	Request getRequest() const override;

private:
	HttpParser parser;
	HttpRequest request;
};
