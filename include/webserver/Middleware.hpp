#pragma once

#include <cstdio>
#include <functional>

#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"

void loggingMiddleware(HTTPRequest& request, HTTPResponse&, std::function<void()> next);
void errorHandlingMiddleware(HTTPRequest& request, HTTPResponse& response, std::function<void()> next);
