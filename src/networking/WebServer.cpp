#include "networking/WebServer.hpp"

#include <csignal>
#include <cstring>
#include <iostream>
#include <sstream>

WebServer::WebServer(const std::string& address, int port, std::unique_ptr<EventPoller> poller) : poller(std::move(poller)), serverSocket(AF_INET, SOCK_STREAM, 0)
{
	serverSocket.setNonBlocking();
	serverSocket.bind(address, port);
	serverSocket.listen();

	this->poller->add(serverSocket.getFileDescriptor());
}

WebServer::~WebServer() = default;

void WebServer::run()
{
	while (true)
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

void WebServer::handleClient(int client)
{
	std::array<char, BUFFER_SIZE> buffer{};
	ssize_t bytesRead;

	while ((bytesRead = Socket::recv(client, buffer)) > 0)
	{
		std::cout << buffer.data();
	}
	std::cout << "\n";

	if (bytesRead == -1 && errno != EAGAIN)
	{
		Socket::close(client);
		throw std::runtime_error("Failed to read from client: " + std::string(strerror(errno)));
	}
	else if (bytesRead == 0)
	{
		Socket::close(client);
	}
	else
	{
		std::string response = buildResponse();
		Socket::send(client, response);
	}
}

std::string WebServer::buildResponse() const
{
	std::ostringstream response;
	response << "HTTP/1.1 200 OK\r\n"
			 << "Content-Type: text/plain\r\n"
			 << "Content-Length: 13\r\n"
			 << "\r\n"
			 << "Hello, World!";
	return response.str();
}
