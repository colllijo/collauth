#include "webserver/Middleware.hpp"

#include "http/HTTPMethod.hpp"
#include "http/HTTPStatus.hpp"
#include "logging/Logger.hpp"

void loggingMiddleware(HTTPRequest& request, HTTPResponse&, std::function<void()> next)
{
	Logger::info("Received request: {} {}", httpMethodToString(request.getMethod()), request.getPath());

	next();
}

void errorHandlingMiddleware(HTTPRequest&, HTTPResponse& response, std::function<void()> next)
{
	try
	{
		next();
	}
	catch (const std::exception& e)
	{
		response.setStatusCode(HTTPStatus::INTERNAL_SERVER_ERROR);
		response.setBody("Internal Server Error: " + std::string(e.what()));
	}
}
