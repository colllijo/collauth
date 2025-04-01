#pragma once

#include <functional>
#include <vector>

#include "http/HttpRequest.hpp"
#include "http/HttpResponse.hpp"

using MiddlewareFunc = std::function<void(HttpRequest&, HttpResponse&, std::function<void()>)>;

class MiddlewareManager
{
public:
	void addMiddleware(MiddlewareFunc middleware);

	void execute(HttpRequest& request, HttpResponse& response, std::function<void()> handler) const;

private:
	std::vector<MiddlewareFunc> middlewareChain;
};
