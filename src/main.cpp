#include <cassert>
#include <csignal>
#include <cstdlib>

#include "cryptography/Random.hpp"
#include "events/EpollPoller.hpp"
#include "math/Montgomery.hpp"
#include "math/Number.hpp"
#ifndef USE_EPOLL
#include "events/SelectPoller.hpp"
#endif

#include "logging/Logger.hpp"
#include "signal/SignalHandler.hpp"
#include "webserver/WebServer.hpp"

constexpr int HTTP_PORT = 8080;

int main()
{
	SignalHandler& signalHandler = *SignalHandler::getInstance();

	Number prime(
		"FFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695A9E13641146433FBCC939DCE249B3EF97D2FE363630C75D8F681B202AEC4617AD3DF1ED5D5FD65612433F51F5F066ED0856365553D"
		"ED1AF3B557135E7F57C935984F0C70E0E68B77E2A689DAF3EFE8721DF158A136ADE73530ACCA4F483A797ABC0AB182B324FB61D108A94BB2C8E3FBB96ADAB760D7F4681D4F42A3DE394DF4AE56EDE76372BB190B07"
		"A7C8EE0A6D709E02FCE1CDF7E2ECC03404CD28342F619172FE9CE98583FF8E4F1232EEF28183C3FE3B1B4C6FAD733BB5FCBC2EC22005C58EF1837D1683B2C6F34A26C1B2EFFA886B423861285C97FFFFFFFFFFFFFF"
		"FF",
		16);
	Number generator = 2;

	Number priv = generateRandomNumber(256);
	Number pub = montgomery::modPow(generator, priv, prime);

	Logger::info("Generated private key: {}", priv.toHex());
	Logger::info("Generated public key: {}", pub.toHex());

	try
	{
#ifdef USE_EPOLL
		std::unique_ptr<EventPoller> poller = std::make_unique<EpollPoller>();
#else
		std::unique_ptr<EventPoller> poller = std::make_unique<SelectPoller>();
#endif

		WebServer server("0.0.0.0", HTTP_PORT, std::move(poller));
		signalHandler.bind(SIGINT, [&server](int) { server.stop(); });

		server.registerRoute(HTTPMethod::GET, "/",
							 [](const HTTPRequest&, HTTPResponse& response)
							 {
								 response.headers["Content-Type"] = "text/plain";
								 response.body = "Hello, world!";

								 return response;
							 });

		server.registerRoute(HTTPMethod::POST, "/echo",
							 [](const HTTPRequest& request, HTTPResponse& response)
							 {
								 auto contentType = request.getHeader("Content-Type");
								 if (contentType.has_value())
								 {
									 response.headers["Content-Type"] = contentType.value();
								 }
								 response.body = request.body;
							 });

		server.run();
	}
	catch (const std::exception& e)
	{
		Logger::error("Server error: {}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
