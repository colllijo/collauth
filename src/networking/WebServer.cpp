#include "networking/WebServer.hpp"

#include <csignal>
#include <cstring>

#include "http/HttpRequest.hpp"
#include "http/HttpResponse.hpp"

WebServer::WebServer(const std::string& address, int port, std::unique_ptr<EventPoller> poller) : poller(std::move(poller)), serverSocket(AF_INET, SOCK_STREAM, 0), stopFlag(false)
{
	serverSocket.setNonBlocking();
	serverSocket.bind(address, port);
	serverSocket.listen();

	this->poller->add(serverSocket.getFileDescriptor());
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

void WebServer::registerRoute(HttpMethod method, const std::string& path, RouteHandler handler)
{
	router.registerRoute(method, path, handler);
}

void WebServer::handleClient(int client)
{
	std::string data;

	std::array<char, BUFFER_SIZE> buffer{};
	ssize_t bytesRead;

	while ((bytesRead = Socket::recv(client, buffer)) > 0)
	{
		data.append(buffer.data(), bytesRead);
	}

	if (bytesRead == -1 && errno != EAGAIN)
	{
		Socket::close(client);
		throw std::runtime_error("Failed to read from client: " + std::string(strerror(errno)));
	}
	else if (bytesRead == 0)
	{
		Socket::close(client);
		return;
	}

	HttpRequest request;
	if (request.parse(data))
	{
		HttpResponse response(HttpStatus::OK);
		response.headers["Server"] = "CollServer";
		response.headers["Content-Type"] = "text/plain";

		router.handleRequest(request, response);
		Socket::send(client, response.build());
	}
	else
	{
		HttpResponse response(HttpStatus::BAD_REQUEST);
		Socket::send(client, response.build());
	}

	Socket::close(client);
}
