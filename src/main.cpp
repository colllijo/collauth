#include <cassert>
#include <csignal>
#include <cstdlib>

#include "cryptography/ecc/MontgomeryCurve.hpp"
#include "cryptography/ecc/Point.hpp"
#include "events/EpollPoller.hpp"
#ifndef USE_EPOLL
#include "events/SelectPoller.hpp"
#endif

#include "logging/Logger.hpp"
#include "signal/SignalHandler.hpp"
#include "webserver/WebServer.hpp"

constexpr int HTTP_PORT = 8080;

int main(int, char** argv)
{
	SignalHandler& signalHandler = *SignalHandler::getInstance();

	Number prime = Number(2).pow(255) - 19;
	Logger::info("Computed prime for curve25519: {}", prime);
	MontgomeryCurve curve25519(486662, 1, prime);
	Point G(9, 0, 1);
	Logger::info("MontgomeryCurve initialized");

	Number priv = Number("77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a", 16);
	priv = Number(argv[1]);
	Logger::info("Calculating public key for private key: {}", priv);
	auto pub = curve25519.scalarMultiply(G, priv);

	Logger::info("Pub Point: ({}, {})", pub.x.toHexString(), pub.z.toHexString());

	Number Z_prime = pub.z.modPow(prime - 2, prime);
	Number publicKey = (pub.x * Z_prime) % prime;

	Logger::info("Public key: {}", publicKey.toHexString());

	Point test(6400, 0, Number("57894719725999370035306716613809057224872645803389535971048342734787925792161"));
	Number zetti = test.z.modPow(prime - 2, prime);
	Number xetti = (test.x * zetti) % prime;

	Logger::info("Test: {}", xetti);

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
