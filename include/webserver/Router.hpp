#pragma once

#include <functional>
#include <map>
#include <utility>

#include "http/HTTPMethod.hpp"
#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"

using RouteHandler = std::function<void(const HTTPRequest&, HTTPResponse&)>;

class Router
{
public:
	void registerRoute(HTTPMethod method, const std::string& path, RouteHandler handler);

	void handleRequest(const HTTPRequest& request, HTTPResponse& response) const;

private:
	std::map<std::pair<HTTPMethod, std::string>, RouteHandler> routes;
};
