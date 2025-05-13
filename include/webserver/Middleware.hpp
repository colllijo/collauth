#pragma once

#include <functional>

#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"

void loggingMiddleware(HTTPRequest& request, HTTPResponse& /*_*/, const std::function<void()>& next);
void errorHandlingMiddleware(HTTPRequest& request, HTTPResponse& response, const std::function<void()>& next);
