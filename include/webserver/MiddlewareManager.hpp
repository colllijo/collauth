#pragma once

#include <functional>
#include <vector>

#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"

using MiddlewareFunc = std::function<void(HTTPRequest&, HTTPResponse&, std::function<void()>)>;

class MiddlewareManager
{
public:
	void addMiddleware(MiddlewareFunc middleware);

	void execute(HTTPRequest& request, HTTPResponse& response, std::function<void()> handler) const;

private:
	std::vector<MiddlewareFunc> middlewareChain;
};
