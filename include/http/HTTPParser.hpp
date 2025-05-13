#pragma once

#include <cstdint>
#include <string>

#include "http/HTTPRequest.hpp"

class HTTPParser
{
public:
	enum class State : uint8_t
	{
		REQUEST_LINE,
		HEADERS,
		BODY,
		DONE
	};

	HTTPParser();

	bool parse(const std::string& data);
	void reset();

	bool hasError() const;
	HTTPRequest getRequest() const;

private:
	State state;
	std::string buffer;
	HTTPRequest request;
	bool error;

	bool parseRequestLine();
	bool parseHeaders();
	bool parseBody();
};
