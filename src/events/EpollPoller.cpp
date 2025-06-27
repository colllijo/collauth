#include "events/EpollPoller.hpp"

#include <cerrno>
#include <cstring>
#include <stdexcept>

#include "events/EventPoller.hpp"

EpollPoller::EpollPoller() : EventPoller()
{
	epollfd = epoll_create1(0);
	if (epollfd == -1)
	{
		throw std::runtime_error("Failed to create epoll instance: " + std::string(strerror(errno)));
	}
}

EpollPoller::~EpollPoller()
{
	if (epollfd != -1)
	{
		close(epollfd);
	}
}

void EpollPoller::add(int fd)
{
	struct epoll_event event{};
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = fd;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		if (errno != EEXIST)
		{
			throw std::runtime_error("Failed to add file descriptor to epoll instance: " + std::string(strerror(errno)));
		}
	}
}

void EpollPoller::remove(int fd)
{
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
}

std::vector<int> EpollPoller::wait() const
{
	std::vector<epoll_event> events(MAX_EVENTS);

	int eventCount = epoll_wait(epollfd, events.data(), MAX_EVENTS, -1);
	if (eventCount == -1)
	{
		if (errno == EINTR)
		{
			return {};
		}

		throw std::runtime_error("Failed to wait for events: " + std::string(strerror(errno)));
	}

	std::vector<int> readyEvents(eventCount);
	for (int i = 0; i < eventCount; ++i)
	{
		readyEvents.push_back(events[i].data.fd);
	}

	return readyEvents;
}
