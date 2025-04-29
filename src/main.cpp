#include <cerrno>
#include <csignal>
#include <cstdlib>

#include "events/EpollPoller.hpp"
#ifndef USE_EPOLL
#include "events/SelectPoller.hpp"
#endif

#include "logging/Logger.hpp"
#include "signal/SignalHandler.hpp"
#include "webserver/WebServer.hpp"

constexpr int HTTP_PORT = 8080;

int main()
{
	SignalHandler& signalHandler = *SignalHandler::getInstance();

	return 1;

	try
	{
#ifdef USE_EPOLL
		std::unique_ptr<EventPoller> poller = std::make_unique<EpollPoller>();
#else
		std::unique_ptr<EventPoller> poller = std::make_unique<SelectPoller>();
#endif

		WebServer server("0.0.0.0", HTTP_PORT, std::move(poller));
		signalHandler.bind(SIGINT, [&server](int) { server.stop(); });

		server.registerRoute(HTTPMethod::GET, "/",
							 [](const HTTPRequest&, HTTPResponse& response)
							 {
								 response.headers["Content-Type"] = "text/plain";
								 response.body = "Hello, world!";

								 return response;
							 });

		server.registerRoute(HTTPMethod::POST, "/echo",
							 [](const HTTPRequest& request, HTTPResponse& response)
							 {
								 auto contentType = request.getHeader("Content-Type");
								 if (contentType.has_value())
								 {
									 response.headers["Content-Type"] = contentType.value();
								 }
								 response.body = request.body;
							 });

		server.run();
	}
	catch (const std::exception& e)
	{
		Logger::error("Server error: {}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
