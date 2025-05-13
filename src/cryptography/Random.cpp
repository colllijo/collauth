#include "cryptography/Random.hpp"

#include <sys/random.h>

#include <stdexcept>

std::vector<uint8_t> generateRandomBytes(size_t length)
{
	std::vector<uint8_t> bytes(length);
	size_t offset = 0;

	while (offset < length)
	{
		ssize_t bytesGenerated = getrandom(bytes.data() + offset, length - offset, 0);

		if (bytesGenerated > 0)
		{
			offset += bytesGenerated;
		}
		else if (bytesGenerated == -1)
		{
			if (errno == EINTR || errno == EAGAIN)
			{
				continue;
			}

			throw std::runtime_error("Failed to generate random bytes");
		}
	}

	return bytes;
}
