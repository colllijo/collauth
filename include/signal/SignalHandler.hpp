#pragma once

#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>

class SignalHandler
{
public:
	using SignalCallback = std::function<void(int)>;

	SignalHandler(const SignalHandler&) = delete;
	SignalHandler& operator=(const SignalHandler&) = delete;

	static SignalHandler* getInstance();

	void bind(int signal, SignalCallback callback);

	static void signalHandler(int signal);

private:
	static SignalHandler* instance;
	static std::mutex mutex;

	std::unordered_map<int, std::vector<SignalCallback>> signalCallbacks;

	SignalHandler();

	void registerHandler(int signal);
};
