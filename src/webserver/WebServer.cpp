#include "webserver/WebServer.hpp"

#include <memory>

#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"
#include "logging/Logger.hpp"
#include "networking/ConnectionFactory.hpp"
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
	Logger::info("WebServer is running on {}:{}", "127.0.0.1", "8080");
	while (!stopFlag.load())
	{
		for (int fd : poller->wait())
		{
			if (fd == httpSocket.getFileDescriptor())
			{
				addConnection(httpSocket.accept(), Protocol::HTTP);
			}
			else if (fd == httpsSocket.getFileDescriptor())
			{
				addConnection(httpsSocket.accept(), Protocol::HTTPS);
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

void WebServer::registerMiddleware(const MiddlewareFunc& middleware)
{
	middlewareManager.addMiddleware(middleware);
}

void WebServer::registerRoute(HTTPMethod method, const std::string& path, const RouteHandler& handler)
{
	router.registerRoute(method, path, handler);
}

void WebServer::addConnection(int fd, Protocol protocol)
{
	connections[fd] = ConnectionFactory::createConnection(fd, protocol);
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
		return;
	}

	std::shared_ptr<Connection> connection = connections.at(client);
	if (!connection->handle())
	{
		removeConnection(client);
		return;
	}

	if (connection->isRequestReady())
	{
		HTTPRequest request = std::get<HTTPRequest>(connection->getRequest());

		HTTPResponse response(HTTPStatus::OK);
		response.headers["Server"] = "CollServer";
		response.headers["Content-Type"] = "text/plain";

		middlewareManager.execute(request, response, [&]() { router.handleRequest(request, response); });
		Socket::send(client, response.build());

		removeConnection(client);
	}
}
