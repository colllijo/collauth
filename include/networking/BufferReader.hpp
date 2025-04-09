#pragma once

#include <cstdint>
#include <vector>
class BufferReader
{
public:
	explicit BufferReader(const std::vector<uint8_t>& data);

	uint8_t readUint8();
	uint16_t readUint16();
	uint32_t readUint32();

	std::vector<uint8_t> readBytes(size_t length);

	bool complete() const;

private:
	const std::vector<uint8_t>& buffer;
	size_t offset;

	void ensureAvailable(size_t length);
};
