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
		std::string response = HttpResponse::generateResponse(request);
		Socket::send(client, response);
	}
	else
	{
		std::string response = "HTTP/1.1 400 Bad Request\r\n\r\n";
		Socket::send(client, response);
	}

	Socket::close(client);
}
