#include "http/HttpMethod.hpp"

HttpMethod parseHttpMethod(const std::string& method)
{
	if (method == "GET")
	{
		return HttpMethod::GET;
	}
	else if (method == "POST")
	{
		return HttpMethod::POST;
	}
	else if (method == "PUT")
	{
		return HttpMethod::PUT;
	}
	else if (method == "DELETE")
	{
		return HttpMethod::DELETE;
	}
	else if (method == "HEAD")
	{
		return HttpMethod::HEAD;
	}
	else if (method == "OPTIONS")
	{
		return HttpMethod::OPTIONS;
	}
	else if (method == "PATCH")
	{
		return HttpMethod::PATCH;
	}
	else
	{
		return HttpMethod::UNKNOWN;
	}
}
