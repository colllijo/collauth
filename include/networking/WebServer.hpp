#pragma once

#include <memory>
#include <string>

#include "events/EventPoller.hpp"
#include "networking/Socket.hpp"

class WebServer
{
public:
	explicit WebServer(const std::string& address, int port, std::unique_ptr<EventPoller> poller);
	~WebServer();

	void run();

private:
	std::unique_ptr<EventPoller> poller;
	Socket serverSocket;

	void handleClient(int client);

	std::string buildResponse() const;
};
