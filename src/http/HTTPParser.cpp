#include "http/HTTPParser.hpp"

#include <sstream>

HTTPParser::HTTPParser() : state(State::REQUEST_LINE), error(false) {}

bool HTTPParser::parse(const std::string& data)
{
	if (error)
	{
		return false;
	}

	buffer += data;

	while (!error && state != State::DONE)
	{
		switch (state)
		{
		case State::REQUEST_LINE:
		{
			bool success = parseRequestLine();
			if (!success)
			{
				return false;
			}
			break;
		}
		case State::HEADERS:
		{
			if (!parseHeaders())
			{
				return false;
			}
			break;
		}
		case State::BODY:
		{
			if (!parseBody())
			{
				return false;
			}
			break;
		}
		case State::DONE:
			break;
		}
	}

	return state == State::DONE;
}

bool HTTPParser::parseRequestLine()
{
	size_t pos = buffer.find("\r\n");
	if (pos == std::string::npos)
	{
		return false;
	}

	std::string line = buffer.substr(0, pos), methodStr, versionStr;
	std::istringstream stream(line);

	if (!(stream >> methodStr >> request.path >> versionStr))
	{
		error = true;
		return false;
	}

	request.method = parseHTTPMethod(methodStr);
	request.version = parseHTTPVersion(versionStr);

	if (request.method == HTTPMethod::UNKNOWN || request.version == HTTPVersion::UNKNOWN)
	{
		error = true;
		return false;
	}

	buffer.erase(0, pos + 2);
	state = State::HEADERS;

	return true;
}

bool HTTPParser::parseHeaders()
{
	size_t pos;
	while ((pos = buffer.find("\r\n")) != std::string::npos)
	{
		// End of headers
		if (pos == 0)
		{
			buffer.erase(0, 2);
			state = State::BODY;
			return true;
		}

		std::string line = buffer.substr(0, pos);
		auto colon = line.find(':');
		if (colon == std::string::npos)
		{
			error = true;
			return false;
		}

		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 1);
		value.erase(0, value.find_first_not_of(" \t"));

		request.setHeader(key, value);
		buffer.erase(0, pos + 2);
	}

	return false;
}

bool HTTPParser::parseBody()
{
	auto contentLengthHeader = request.getHeader("Content-Length");
	if (contentLengthHeader.has_value())
	{
		int contentLength = std::stoi(contentLengthHeader.value());
		if (buffer.size() < static_cast<size_t>(contentLength))
		{
			return false;
		}

		request.body = buffer.substr(0, contentLength);
		buffer.erase(0, contentLength);
	}

	state = State::DONE;
	return true;
}

HTTPRequest HTTPParser::getRequest() const
{
	return request;
}

bool HTTPParser::hasError() const
{
	return error;
}

void HTTPParser::reset()
{
	state = State::REQUEST_LINE;
	buffer.clear();
	request = HTTPRequest();
	error = false;
}
