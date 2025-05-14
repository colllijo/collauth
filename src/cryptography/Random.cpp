#include "cryptography/Random.hpp"

#include <sys/random.h>

#include <cstdlib>
#include <stdexcept>

namespace
{
	std::vector<uint32_t> bytesToDigits(const std::vector<uint8_t>& bytes);
}

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

Number generateRandomNumber(const Number& max)
{
	if (max <= 0)
	{
		throw std::invalid_argument("Max must be greater than 0");
	}

	size_t byteLength = (max.bitLength() + 7) / 8;
	std::vector<uint8_t> randomBytes = generateRandomBytes(byteLength);

	// Ensure the generated number is less than max
	Number randomNumber(bytesToDigits(randomBytes));
	while (randomNumber >= max)
	{
		randomBytes = generateRandomBytes(byteLength);
		randomNumber = Number(bytesToDigits(randomBytes));
	}

	return randomNumber;
}

namespace
{
	std::vector<uint32_t> bytesToDigits(const std::vector<uint8_t>& bytes)
	{
		std::vector<uint32_t> digits((bytes.size() + 3) / 4);
		for (size_t i = 0; i < bytes.size(); ++i)
		{
			digits[i / 4] |= static_cast<uint32_t>(bytes[i]) << ((i % 4) * 8);
		}
		return digits;
	}
}  // namespace
