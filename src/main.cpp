#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <iostream>

#include "events/EpollPoller.hpp"
#include "events/SelectPoller.hpp"
#include "networking/WebServer.hpp"

constexpr int PORT = 8080;

int main()
{
	try
	{
#ifdef USE_EPOLL
		std::unique_ptr<EventPoller> poller = std::make_unique<EpollPoller>();
#else
		std::unique_ptr<EventPoller> poller = std::make_unique<SelectPoller>();
#endif

		WebServer server("0.0.0.0", PORT, std::move(poller));
		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Server error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
