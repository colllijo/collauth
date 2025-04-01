#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <iostream>

#include "events/EpollPoller.hpp"
#include "events/SelectPoller.hpp"
#include "networking/WebServer.hpp"
#include "signal/SignalHandler.hpp"

constexpr int PORT = 8080;

// void signalHandler(int signal)
// {
// 	std::cout << "Received signal " << signal << ". Stopping server..." << std::endl;
// }

int main()
{
	SignalHandler& signalHandler = *SignalHandler::getInstance();

	// std::signal(SIGINT, signalHandler);

	try
	{
#ifdef USE_EPOLL
		std::unique_ptr<EventPoller> poller = std::make_unique<EpollPoller>();
#else
		std::unique_ptr<EventPoller> poller = std::make_unique<SelectPoller>();
#endif

		WebServer server("0.0.0.0", PORT, std::move(poller));
		signalHandler.bind(SIGINT, [&server](int) { server.stop(); });

		server.registerRoute(HttpMethod::GET, "/", [](const HttpRequest&, HttpResponse& response) {
			response.headers["Content-Type"] = "text/plain";
			response.body = "Hello, world!";

			return response;
		});

		server.registerRoute(HttpMethod::POST, "/echo", [](const HttpRequest& request, HttpResponse& response) {
			if (request.headers.contains("Content-Type"))
			{
				response.headers["Content-Type"] = request.headers.at("Content-Type");
			}
			response.body = request.body;
		});

		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Server error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
