#ifdef __linux__

#define USE_EPOLL

#pragma once

#include <sys/epoll.h>
#include <unistd.h>

#include "events/EventPoller.hpp"

constexpr int MAX_EVENTS = 128;

class EpollPoller : public EventPoller
{
public:
	EpollPoller();
	~EpollPoller() override;

	void add(int fd) override;
	void remove(int fd) override;

	std::vector<int> wait() const override;

private:
	int epollfd;
};

#endif
