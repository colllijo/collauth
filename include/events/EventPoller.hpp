#pragma once

#include <vector>

class EventPoller
{
public:
	EventPoller() = default;
	virtual ~EventPoller() = default;

	EventPoller(const EventPoller&) = delete;
	EventPoller& operator=(const EventPoller&) = delete;

	EventPoller(EventPoller&&) = delete;
	EventPoller& operator=(EventPoller&&) = delete;

	virtual void add(int fd) = 0;
	virtual void remove(int fd) = 0;

	virtual std::vector<int> wait() const = 0;
};
