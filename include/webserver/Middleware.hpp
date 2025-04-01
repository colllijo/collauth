#pragma once

#include <cstdio>
#include <functional>

#include "http/HttpRequest.hpp"
#include "http/HttpResponse.hpp"

void loggingMiddleware(HttpRequest& request, HttpResponse&, std::function<void()> next);
void errorHandlingMiddleware(HttpRequest& request, HttpResponse& response, std::function<void()> next);
