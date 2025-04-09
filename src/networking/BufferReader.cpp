#include "networking/BufferReader.hpp"

#include <stdexcept>
#include <vector>

BufferReader::BufferReader(const std::vector<uint8_t>& data) : buffer(data), offset(0) {}

uint8_t BufferReader::readUint8()
{
	ensureAvailable(1);

	return buffer[offset++];
}

uint16_t BufferReader::readUint16()
{
	ensureAvailable(2);

	uint16_t value = (buffer[offset] << 8) | buffer[offset + 1];
	offset += 2;

	return value;
}

uint32_t BufferReader::readUint32()
{
	ensureAvailable(4);

	uint32_t value = (buffer[offset] << 24) | (buffer[offset + 1] << 16) | (buffer[offset + 2] << 8) | buffer[offset + 3];
	offset += 4;

	return value;
}

std::vector<uint8_t> BufferReader::readBytes(size_t length)
{
	ensureAvailable(length);

	std::vector<uint8_t> bytes(buffer.begin() + offset, buffer.begin() + offset + length);
	offset += length;

	return bytes;
}

bool BufferReader::complete() const
{
	return offset == buffer.size();
}

void BufferReader::ensureAvailable(size_t length)
{
	if (offset + length > buffer.size())
	{
		throw std::out_of_range("Not enough data in buffer");
	}
}
