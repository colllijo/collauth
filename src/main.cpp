#include <iostream>
#include "networking/Socket.hpp"

int main()
{
	Socket server(AF_INET, SOCK_STREAM, 0);
	server.bind("0.0.0.0", 8080);
	server.listen();

	Socket client = server.accept();

	std::string buffer;
	while (client.recv(buffer))
	{
		std::cout << buffer;
	}

	std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += "Hello, world!";
	server.send(response);

	return 0;
}
