#include "networking/Socket.hpp"

#include <arpa/inet.h>
#include <unistd.h>

#include <cstddef>
#include <stdexcept>

Socket::Socket(int domain, int type, int protocol) : domain(domain)
{
	if (((AF_INET | AF_INET6) & domain) == 0)
	{
		throw std::runtime_error("Invalid domain");
	}

	fileDescriptor = ::socket(domain, type, protocol);

	if (fileDescriptor < 0)
	{
		throw std::runtime_error("Failed to create socket");
	}
}

Socket::Socket(int domain, int fileDescriptor) : domain(domain), fileDescriptor(fileDescriptor)
{
	if (fileDescriptor < 0)
	{
		throw std::runtime_error("Failed to create socket");
	}
}

Socket::~Socket()
{
	if (fileDescriptor != -1)
	{
		::close(fileDescriptor);
	}
}

void Socket::bind(const std::string& address, int port) const
{
	switch (domain)
	{
	case AF_INET:
		bindIPv4(address, port);
		break;
	case AF_INET6:
		bindIPv6(address, port);
		break;
	}
}

void Socket::listen(int backlog) const
{
	if (::listen(fileDescriptor, backlog) < 0)
	{
		throw std::runtime_error("Failed to listen on socket");
	}
}

Socket Socket::accept(sockaddr_in* client_addr) const
{
	socklen_t client_addr_len = sizeof(*client_addr);
	int client_fd = ::accept(fileDescriptor, (struct sockaddr*)client_addr, &client_addr_len);

	if (client_fd < 0)
	{
		throw std::runtime_error("Failed to accept connection");
	}

	return Socket(domain, client_fd);
}

void Socket::send(const std::string& data) const
{
	if (::send(fileDescriptor, data.c_str(), data.size(), 0) < 0)
	{
		throw std::runtime_error("Failed to send data");
	}
}

bool Socket::recv(std::string& buffer, int bufferSize) const
{
	char* data = new char[bufferSize];
	long bytes_received = ::recv(fileDescriptor, data, bufferSize, 0);

	if (bytes_received < 0)
	{
		delete[] data;
		throw std::runtime_error("Failed to receive data");
	}

	buffer = std::string(data, bytes_received);
	delete[] data;

	return bytes_received > 0;
}

void Socket::bindIPv4(const std::string& address, int port) const
{
	struct sockaddr_in server_addr = {};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr);

	if (::bind(fileDescriptor, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		throw std::runtime_error("Failed to bind socket");
	}
}

void Socket::bindIPv6(const std::string& address, int port) const
{
	struct sockaddr_in6 server_addr = {};
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(port);

	inet_pton(AF_INET6, address.c_str(), &server_addr.sin6_addr);

	if (::bind(fileDescriptor, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		throw std::runtime_error("Failed to bind socket");
	}
}
