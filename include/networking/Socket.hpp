#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

#include <string>

class Socket
{
public:
	Socket(int domain, int type, int protocol);
	~Socket();

	void bind(const std::string& address, int port) const;
	void listen(int backlog = SOMAXCONN) const;

	Socket accept(sockaddr_in* client_addr = nullptr) const;

	void send(const std::string& data) const;
	bool recv(std::string& buffer, int bufferSize = 1024) const;

private:
	int domain;

	int fileDescriptor;

	Socket(int domain, int fileDescriptor);

	void bindIPv4(const std::string& address, int port) const;
	void bindIPv6(const std::string& address, int port) const;
};
