#include "http/HttpVersion.hpp"

HttpVersion parseHttpVersion(const std::string& version)
{
	if (version == "HTTP/1.0")
	{
		return HttpVersion::HTTP_1_0;
	}
	else if (version == "HTTP/1.1")
	{
		return HttpVersion::HTTP_1_1;
	}
	else if (version == "HTTP/2.0")
	{
		return HttpVersion::HTTP_2_0;
	}
	else
	{
		return HttpVersion::UNKNOWN;
	}
}
