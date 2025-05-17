#pragma once

#include <vector>

class EventPoller
{
public:
	EventPoller() = default;
	virtual ~EventPoller() = default;

	virtual void add(int fd) = 0;
	virtual void remove(int fd) = 0;

	virtual std::vector<int> wait() const = 0;
};
