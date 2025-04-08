#include "http/HTTPVersion.hpp"

HTTPVersion parseHTTPVersion(const std::string& version)
{
	if (version == "HTTP/1.0")
	{
		return HTTPVersion::HTTP_1_0;
	}
	else if (version == "HTTP/1.1")
	{
		return HTTPVersion::HTTP_1_1;
	}
	else if (version == "HTTP/2.0")
	{
		return HTTPVersion::HTTP_2_0;
	}
	else
	{
		return HTTPVersion::UNKNOWN;
	}
}
