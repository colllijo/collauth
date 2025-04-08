#include "webserver/Middleware.hpp"

#include "http/HttpMethod.hpp"
#include "http/HttpStatus.hpp"
#include "logging/Logger.hpp"

void loggingMiddleware(HttpRequest& request, HttpResponse&, std::function<void()> next)
{
	Logger::info("Received request: {} {}", httpMethodToString(request.getMethod()), request.getPath());

	next();
}

void errorHandlingMiddleware(HttpRequest&, HttpResponse& response, std::function<void()> next)
{
	try
	{
		next();
	}
	catch (const std::exception& e)
	{
		response.setStatusCode(HttpStatus::INTERNAL_SERVER_ERROR);
		response.setBody("Internal Server Error: " + std::string(e.what()));
	}
}
