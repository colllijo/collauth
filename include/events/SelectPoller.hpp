#pragma once

#include <sys/select.h>

#include "events/EventPoller.hpp"

class SelectPoller : public EventPoller
{
public:
	SelectPoller();

	void add(int fd) override;
	void remove(int fd) override;

	std::vector<int> wait() const override;

private:
	fd_set masterSet;
	int maxFd;
};
