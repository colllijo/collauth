#pragma once

#include <atomic>
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
	void stop();

private:
	std::unique_ptr<EventPoller> poller;
	Socket serverSocket;

	std::atomic<bool> stopFlag;

	void handleClient(int client);
};
