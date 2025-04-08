#include "webserver/WebServer.hpp"

#include <csignal>
#include <cstring>
#include <memory>

#include "http/HttpRequest.hpp"
#include "http/HttpResponse.hpp"
#include "logging/Logger.hpp"
#include "webserver/Middleware.hpp"

WebServer::WebServer(const std::string& address, int port, std::unique_ptr<EventPoller> poller)
	: poller(std::move(poller)), httpSocket(AF_INET, SOCK_STREAM, 0), httpsSocket(AF_INET, SOCK_STREAM, 0), stopFlag(false)
{
	httpSocket.setSocketOption(SO_REUSEADDR, true);
	httpSocket.setNonBlocking();
	httpSocket.bind(address, port);
	httpSocket.listen();

	this->poller->add(httpSocket.getFileDescriptor());

	httpsSocket.setSocketOption(SO_REUSEADDR, true);
	httpsSocket.setNonBlocking();
	httpsSocket.bind(address, 8443);
	httpsSocket.listen();

	this->poller->add(httpsSocket.getFileDescriptor());

	middlewareManager.addMiddleware(errorHandlingMiddleware);
	middlewareManager.addMiddleware(loggingMiddleware);
}

WebServer::~WebServer()
{
	this->poller->remove(httpSocket.getFileDescriptor());
	for (const auto& [fd, connection] : connections)
	{
		this->poller->remove(fd);
		Socket::close(fd);
	}
	connections.clear();
};

void WebServer::run()
{
	while (!stopFlag.load())
	{
		auto readyFds = poller->wait();

		for (int fd : readyFds)
		{
			if (fd == httpSocket.getFileDescriptor())
			{
				int clientFd = httpSocket.accept();

				addConnection(clientFd);
			}
			else if (fd == httpsSocket.getFileDescriptor())
			{
				int clientFd = httpsSocket.accept();

				addConnection(clientFd, true);
			}
			else
			{
				handleClient(fd);
			}
		}
	}
}

void WebServer::stop()
{
	stopFlag.store(true);
}

void WebServer::registerMiddleware(MiddlewareFunc middleware)
{
	middlewareManager.addMiddleware(middleware);
}

void WebServer::registerRoute(HttpMethod method, const std::string& path, RouteHandler handler)
{
	router.registerRoute(method, path, handler);
}

void WebServer::addConnection(int fd, bool tls)
{
	connections[fd] = std::make_shared<Connection>(fd, tls);
	poller->add(fd);
}

void WebServer::removeConnection(int fd)
{
	connections.erase(fd);
	poller->remove(fd);
}

void WebServer::handleClient(int client)
{
	if (!connections.contains(client))
	{
		Logger::debug("Client not found: {}", client);
		return;
	}

	std::shared_ptr<Connection> connection = connections.at(client);
	if (!connection->handleRead())
	{
		removeConnection(client);
		return;
	}

	if (connection->isRequestReady())
	{
		HttpRequest request = connection->getRequest();

		HttpResponse response(HttpStatus::OK);
		response.headers["Server"] = "CollServer";
		response.headers["Content-Type"] = "text/plain";

		middlewareManager.execute(request, response, [&]() { router.handleRequest(request, response); });
		Socket::send(client, response.build());

		removeConnection(client);
	}
}
