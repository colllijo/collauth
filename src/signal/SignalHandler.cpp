#include "signal/SignalHandler.hpp"

#include <csignal>

SignalHandler* SignalHandler::instance = nullptr;
std::mutex SignalHandler::mutex;

SignalHandler::SignalHandler() : signalCallbacks() {}

SignalHandler* SignalHandler::getInstance()
{
	if (instance == nullptr)
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (instance == nullptr)
		{
			instance = new SignalHandler();
		}
	}
	return instance;
}

void SignalHandler::bind(int signal, SignalCallback callback)
{
	if (signalCallbacks.find(signal) == signalCallbacks.end())
	{
		registerHandler(signal);
	}

	signalCallbacks[signal].push_back(std::move(callback));
}

void SignalHandler::registerHandler(int signal)
{
	std::signal(signal, SignalHandler::signalHandler);
}

void SignalHandler::signalHandler(int signal)
{
	SignalHandler* signalHandler = SignalHandler::getInstance();
	if (signalHandler->signalCallbacks.find(signal) != signalHandler->signalCallbacks.end())
	{
		for (const auto& callback : signalHandler->signalCallbacks[signal])
		{
			callback(signal);
		}
	}
}
