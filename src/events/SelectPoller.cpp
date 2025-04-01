#include "events/SelectPoller.hpp"

#include <cstring>
#include <stdexcept>

SelectPoller::SelectPoller(): masterSet(), maxFd(0)
{
}

void SelectPoller::add(int fd)
{
	FD_SET(fd, &masterSet);
	maxFd = std::max(maxFd, fd);
}

void SelectPoller::remove(int fd)
{
	FD_CLR(fd, &masterSet);
}

std::vector<int> SelectPoller::wait() const
{
	fd_set readSet = masterSet;
	int result = select(maxFd + 1, &readSet, nullptr, nullptr, nullptr);

	if (result == -1)
	{
		if (errno == EINTR)
		{
			return {};
		}

		throw std::runtime_error("select() failed: " + std::string(strerror(errno)));
	}

	std::vector<int> readyFds;
	for (int fd = 0; fd <= maxFd; ++fd)
	{
		if (FD_ISSET(fd, &readSet))
		{
			readyFds.push_back(fd);
		}
	}

	return readyFds;
}
