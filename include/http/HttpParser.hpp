#pragma once

#include <string>

#include "http/HttpRequest.hpp"

class HttpParser
{
public:
	enum class State
	{
		REQUEST_LINE,
		HEADERS,
		BODY,
		DONE
	};

	HttpParser();

	bool parse(const std::string& data);
	void reset();

	bool hasError() const;
	HttpRequest getRequest() const;

private:
	State state;
	std::string buffer;
	HttpRequest request;
	bool error;

	bool parseRequestLine();
	bool parseHeaders();
	bool parseBody();
};
