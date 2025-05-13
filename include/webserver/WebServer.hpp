#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>

#include "events/EventPoller.hpp"
#include "networking/Connection.hpp"
#include "networking/ConnectionFactory.hpp"
#include "networking/Socket.hpp"
#include "webserver/MiddlewareManager.hpp"
#include "webserver/Router.hpp"

class WebServer
{
public:
	explicit WebServer(const std::string& address, int port, std::unique_ptr<EventPoller> poller);
	~WebServer();

	WebServer(const WebServer&) = delete;
	WebServer& operator=(const WebServer&) = delete;

	WebServer(WebServer&&) noexcept = delete;
	WebServer& operator=(WebServer&&) noexcept = delete;

	void run();
	void stop();

	void registerMiddleware(const MiddlewareFunc& middleware);
	void registerRoute(HTTPMethod method, const std::string& path, const RouteHandler& handler);

private:
	std::unique_ptr<EventPoller> poller;
	Socket httpSocket;
	Socket httpsSocket;

	std::atomic<bool> stopFlag;

	std::unordered_map<int, std::shared_ptr<Connection>> connections;

	Router router;
	MiddlewareManager middlewareManager;

	void handleClient(int client);

	void addConnection(int fd, Protocol protocol);
	void removeConnection(int fd);
};
