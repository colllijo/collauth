#pragma once

#include <functional>
#include <map>
#include <utility>

#include "http/HttpMethod.hpp"
#include "http/HttpRequest.hpp"
#include "http/HttpResponse.hpp"

using RouteHandler = std::function<void(const HttpRequest&, HttpResponse&)>;

class Router
{
public:
	void registerRoute(HttpMethod method, const std::string& path, RouteHandler handler);

	void handleRequest(const HttpRequest& request, HttpResponse& response) const;

private:
	std::map<std::pair<HttpMethod, std::string>, RouteHandler> routes;
};
