#include "webserver/Middleware.hpp"

#include "http/HttpMethod.hpp"
#include "http/HttpStatus.hpp"

void loggingMiddleware(HttpRequest& request, HttpResponse&, std::function<void()> next)
{
	printf("Request: %s %s\n", httpMethodToString(request.getMethod()).c_str(), request.getPath().c_str());
	next();
}

void errorHandlingMiddleware(HttpRequest&, HttpResponse& response, std::function<void()> next)
{
	try {
		next();
	} catch (const std::exception& e) {
		response.setStatusCode(HttpStatus::INTERNAL_SERVER_ERROR);
		response.setBody("Internal Server Error: " + std::string(e.what()));
	}
}
