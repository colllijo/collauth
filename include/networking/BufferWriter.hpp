#pragma once

#include <cstdint>
#include <vector>
class BufferWriter
{
public:
	explicit BufferWriter();

	void writeUint8(uint8_t value);
	void writeUint16(uint16_t value);
	void writeUint24(uint32_t value);
	void writeUint32(uint32_t value);

	void writeBytes(const std::vector<uint8_t>& bytes);

	std::vector<uint8_t> getBuffer() const;

private:
	std::vector<uint8_t> buffer;
};
