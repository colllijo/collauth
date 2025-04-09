#include "networking/BufferWriter.hpp"

BufferWriter::BufferWriter() = default;

void BufferWriter::writeUint8(uint8_t value)
{
	buffer.push_back(value);
}

void BufferWriter::writeUint16(uint16_t value)
{
	buffer.push_back(static_cast<uint8_t>(value >> 8));
	buffer.push_back(static_cast<uint8_t>(value & 0xFF));
}

void BufferWriter::writeUint24(uint32_t value)
{
	buffer.push_back(static_cast<uint8_t>(value >> 16));
	buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
	buffer.push_back(static_cast<uint8_t>(value & 0xFF));
}

void BufferWriter::writeUint32(uint32_t value)
{
	buffer.push_back(static_cast<uint8_t>(value >> 24));
	buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
	buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
	buffer.push_back(static_cast<uint8_t>(value & 0xFF));
}

void BufferWriter::writeBytes(const std::vector<uint8_t>& bytes)
{
	buffer.insert(buffer.end(), bytes.begin(), bytes.end());
}

std::vector<uint8_t> BufferWriter::getBuffer() const
{
	return buffer;
}
