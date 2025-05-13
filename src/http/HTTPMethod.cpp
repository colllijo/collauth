#include "http/HTTPMethod.hpp"

#include <unordered_map>

HTTPMethod parseHTTPMethod(const std::string& method)
{
	if (method == "GET")
	{
		return HTTPMethod::GET;
	}
	else if (method == "POST")
	{
		return HTTPMethod::POST;
	}
	else if (method == "PUT")
	{
		return HTTPMethod::PUT;
	}
	else if (method == "DELETE")
	{
		return HTTPMethod::DELETE;
	}
	else if (method == "HEAD")
	{
		return HTTPMethod::HEAD;
	}
	else if (method == "OPTIONS")
	{
		return HTTPMethod::OPTIONS;
	}
	else if (method == "PATCH")
	{
		return HTTPMethod::PATCH;
	}
	else
	{
		return HTTPMethod::UNKNOWN;
	}
}

std::unordered_map<HTTPMethod, std::string> methodToStringMap = {{HTTPMethod::GET, "GET"},		 {HTTPMethod::POST, "POST"},	  {HTTPMethod::PUT, "PUT"},
																 {HTTPMethod::DELETE, "DELETE"}, {HTTPMethod::HEAD, "HEAD"},	  {HTTPMethod::OPTIONS, "OPTIONS"},
																 {HTTPMethod::PATCH, "PATCH"},	 {HTTPMethod::UNKNOWN, "UNKNOWN"}};

std::string httpMethodToString(HTTPMethod method)
{
	return methodToStringMap[method];
}
