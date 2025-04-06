#include "webserver/WebServer.hpp"

#include <csignal>
#include <cstring>

#include "http/HttpRequest.hpp"
#include "http/HttpResponse.hpp"
#include "logging/Logger.hpp"
#include "webserver/Middleware.hpp"

WebServer::WebServer(const std::string& address, int port, std::unique_ptr<EventPoller> poller) : poller(std::move(poller)), serverSocket(AF_INET, SOCK_STREAM, 0), stopFlag(false)
{
	serverSocket.setSocketOption(SO_REUSEADDR, true);
	serverSocket.setNonBlocking();
	serverSocket.bind(address, port);
	serverSocket.listen();

	this->poller->add(serverSocket.getFileDescriptor());

	middlewareManager.addMiddleware(errorHandlingMiddleware);
	middlewareManager.addMiddleware(loggingMiddleware);
}

WebServer::~WebServer() = default;

void WebServer::run()
{
	while (!stopFlag.load())
	{
		auto readyFds = poller->wait();

		for (int fd : readyFds)
		{
			if (fd == serverSocket.getFileDescriptor())
			{
				int client = serverSocket.accept();
				Socket::setNonBlocking(client);
				poller->add(client);
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

void WebServer::handleClient(int client)
{
	std::string data = Socket::receive(client);

	Logger::log("Received data: {}", data);

	HttpRequest request;
	if (request.parse(data))
	{
		HttpResponse response(HttpStatus::OK);
		response.headers["Server"] = "CollServer";
		response.headers["Content-Type"] = "text/plain";

		middlewareManager.execute(request, response, [&]() { router.handleRequest(request, response); });
		Socket::send(client, response.build());
	}
	else
	{
		HttpResponse response(HttpStatus::BAD_REQUEST);
		Socket::send(client, response.build());
	}

	Socket::close(client);
}
