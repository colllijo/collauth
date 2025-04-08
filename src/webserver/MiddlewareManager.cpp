#include "webserver/MiddlewareManager.hpp"

#include <functional>

void MiddlewareManager::addMiddleware(MiddlewareFunc middleware)
{
	middlewareChain.push_back(middleware);
}

void MiddlewareManager::execute(HTTPRequest& request, HTTPResponse& response, std::function<void()> handler) const
{
	size_t index = 0;

	std::function<void()> next = [&]()
	{
		if (index < middlewareChain.size())
		{
			middlewareChain[index++](request, response, next);
		}
		else
		{
			handler();
		}
	};

	next();
}
