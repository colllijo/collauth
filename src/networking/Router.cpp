#include "networking/Router.hpp"

void Router::registerRoute(HttpMethod method, const std::string& path, RouteHandler handler)
{
	routes[std::make_pair(method, path)] = handler;
}

void Router::handleRequest(const HttpRequest& request, HttpResponse& response) const
{
	auto it = routes.find(std::make_pair(request.method, request.path));
	if (it != routes.end())
	{
		it->second(request, response);
	}
	else
	{
		response.status = HttpStatus::NOT_FOUND;
		response.headers["Content-Type"] = "text/plain";
		response.body = "404 Not Found";
	}
}
