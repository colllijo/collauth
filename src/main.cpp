#include <cassert>
#include <csignal>
#include <cstdlib>

#include "cryptography/Random.hpp"
#include "cryptography/ecc/Point.hpp"
#include "cryptography/ecc/WeierstrassCurve.hpp"
#include "events/EpollPoller.hpp"
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

	auto p = Number("FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF", 16);
	auto a = Number("-3");
	auto b = Number("5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B", 16);

	auto G = Point(Number::fromString("6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296", 16),
				   Number::fromString("4FE342E2FE1A7F9B8EE7EB4A7C0F9E162CBFBA154A4B8E4BA19B0C527D2779C0", 16));

	auto n = Number("FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551", 16);
	auto h = Number("1");

	auto curve = WeierstrassCurve(a, b, p);

	auto privateKey = generateRandomNumber(n);
	privateKey = 1;
	auto publicKey = curve.multiply(G, privateKey);
	Logger::info("Hello, World!");

	Logger::info("Private Key: {}", privateKey);
	Logger::info("Public Key: ({}, {})", publicKey.x, publicKey.y);

	return 1;

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
