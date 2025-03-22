#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

#include <array>
#include <string>

constexpr int BUFFER_SIZE = 1024;

class Socket
{
public:
	explicit Socket(int domain, int type, int protocol);
	~Socket();

	static void setNonBlocking(int sockfd);
	void setNonBlocking();

	void bind(const std::string& address, int port);
	void listen(int backlog = SOMAXCONN);

	int accept(sockaddr_in* client_addr = nullptr);

	static void send(int sockfd, const std::string& data);
	void send(const std::string& data);
	static ssize_t recv(int sockfd, std::array<char, BUFFER_SIZE>& buffer);
	ssize_t recv(std::array<char, BUFFER_SIZE>& buffer);

	static void close(int sockfd);
	void close();

	int getFileDescriptor() const;

private:
	int domain;
	int sockfd;

	void bindIPv4(const std::string& address, int port);
	void bindIPv6(const std::string& address, int port);
};
