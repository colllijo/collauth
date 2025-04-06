#include <cerrno>
#include <csignal>
#include <cstdlib>

#include "events/EpollPoller.hpp"
#include "events/SelectPoller.hpp"
#include "logging/Logger.hpp"
#include "signal/SignalHandler.hpp"
#include "webserver/WebServer.hpp"

constexpr int PORT = 8080;

int main()
{
	SignalHandler& signalHandler = *SignalHandler::getInstance();

	try
	{
#ifdef USE_EPOLL
		std::unique_ptr<EventPoller> poller = std::make_unique<EpollPoller>();
#else
		std::unique_ptr<EventPoller> poller = std::make_unique<SelectPoller>();
#endif

		WebServer server("0.0.0.0", PORT, std::move(poller));
		signalHandler.bind(SIGINT, [&server](int) { server.stop(); });

		server.registerRoute(HttpMethod::GET, "/",
							 [](const HttpRequest&, HttpResponse& response)
							 {
								 response.headers["Content-Type"] = "text/plain";
								 response.body = "Hello, world!";

								 return response;
							 });

		server.registerRoute(HttpMethod::POST, "/echo",
							 [](const HttpRequest& request, HttpResponse& response)
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
