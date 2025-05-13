#include "networking/Socket.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <array>
#include <cstring>
#include <stdexcept>

#include "logging/Logger.hpp"

Socket::Socket(int domain, int type, int protocol) : domain(domain)
{
	if (((AF_INET | AF_INET6) & domain) == 0)
	{
		throw std::runtime_error("Invalid domain");
	}

	sockfd = ::socket(domain, type, protocol);
	if (sockfd == -1)
	{
		throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
	}
}

Socket::Socket(int fileDescriptor) : domain(-1), sockfd(fileDescriptor)
{
	if (sockfd == -1)
	{
		throw std::runtime_error("Invalid file descriptor");
	}
}

Socket::~Socket()
{
	try
	{
		close();
	}
	catch (const std::exception& e)
	{
		Logger::error(std::string(e.what()));
	}
}

void Socket::setSocketOption(int sockfd, int option, int value)
{
	if (::setsockopt(sockfd, SOL_SOCKET, option, &value, sizeof(value)) < 0)
	{
		throw std::runtime_error("Failed to set socket option: " + std::string(strerror(errno)));
	}
}

void Socket::setSocketOption(int option, int value)
{
	setSocketOption(sockfd, option, value);
}

void Socket::setNonBlocking(int sockfd)
{
	int flags = ::fcntl(sockfd, F_GETFL, 0);

	if (flags == -1 || ::fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		throw std::runtime_error("Failed to set socket flags: " + std::string(strerror(errno)));
	}
}

void Socket::setNonBlocking()
{
	setNonBlocking(sockfd);
}

void Socket::bind(const std::string& address, int port)
{
	switch (domain)
	{
	case AF_INET:
		bindIPv4(address, port);
		break;
	case AF_INET6:
		bindIPv6(address, port);
		break;
	default:
		throw std::runtime_error("Invalid domain");
	}
}

void Socket::listen(int backlog)
{
	if (::listen(sockfd, backlog) < 0)
	{
		throw std::runtime_error("Failed to listen on socket: " + std::string(strerror(errno)));
	}
}

int Socket::accept(sockaddr_in* client_addr)
{
	socklen_t client_addr_len = sizeof(*client_addr);
	int client_fd = ::accept(sockfd, reinterpret_cast<sockaddr*>(client_addr), &client_addr_len);

	if (client_fd == -1)
	{
		throw std::runtime_error("Failed to accept connection: " + std::string(strerror(errno)));
	}

	return client_fd;
}

void Socket::send(int sockfd, const std::string& data)
{
	if (::send(sockfd, data.c_str(), data.size(), 0) < 0)
	{
		throw std::runtime_error("Failed to send data: " + std::string(strerror(errno)));
	}
}

void Socket::send(const std::string& data)
{
	send(sockfd, data);
}

void Socket::send(int sockfd, const std::vector<uint8_t>& data)
{
	if (::send(sockfd, data.data(), data.size(), 0) < 0)
	{
		throw std::runtime_error("Failed to send data: " + std::string(strerror(errno)));
	}
}

void Socket::send(const std::vector<uint8_t>& data)
{
	send(sockfd, data);
}

std::string Socket::receive(int sockfd)
{
	std::string data;

	std::array<char, BUFFER_SIZE> buffer{};

	do
	{
		ssize_t bytesReceived = ::recv(sockfd, buffer.data(), BUFFER_SIZE, 0);

		if (bytesReceived == 0)
		{
			break;
		}
		else if (bytesReceived == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				// No more data to read, return
				break;
			}
			else
			{
				throw std::runtime_error("Failed to read from client: " + std::string(strerror(errno)));
			}
		}

		data.append(buffer.data(), bytesReceived);
	} while (true);

	return data;
}

std::string Socket::receive()
{
	return receive(sockfd);
}

ssize_t Socket::recv(int sockfd, std::array<char, BUFFER_SIZE>& buffer)
{
	return ::recv(sockfd, buffer.data(), buffer.size(), 0);
}

ssize_t Socket::recv(std::array<char, BUFFER_SIZE>& buffer)
{
	return recv(sockfd, buffer);
}

void Socket::close(int sockfd)
{
	if (sockfd != -1)
	{
		if (::close(sockfd) < 0)
		{
			throw std::runtime_error("Failed to close socket: " + std::string(strerror(errno)));
		}
	}
}

void Socket::close()
{
	close(sockfd);
	sockfd = -1;
}

int Socket::getFileDescriptor() const
{
	return sockfd;
}

void Socket::bindIPv4(const std::string& address, int port)
{
	struct sockaddr_in server_addr = {};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr);

	if (::bind(sockfd, reinterpret_cast<const sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
	{
		throw std::runtime_error("Failed to bind socket: " + std::string(strerror(errno)));
	}
}

void Socket::bindIPv6(const std::string& address, int port)
{
	struct sockaddr_in6 server_addr = {};
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(port);

	inet_pton(AF_INET6, address.c_str(), &server_addr.sin6_addr);

	if (::bind(sockfd, reinterpret_cast<const sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
	{
		throw std::runtime_error("Failed to bind socket: " + std::string(strerror(errno)));
	}
}
