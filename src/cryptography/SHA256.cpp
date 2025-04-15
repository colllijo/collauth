#include "cryptography/SHA256.hpp"

#include <bit>
#include <format>
#include <string>

SHA256::SHA256()
{
	reset();
}

SHA256& SHA256::update(const std::string& input)
{
	buffer.insert(buffer.end(), input.begin(), input.end());

	return *this;
}

std::string SHA256::digest()
{
	pad();
	processChunks();
	return toHex();
}

SHA256::Word SHA256::rotr(Word x, Word n)
{
	return std::rotr(x, n);
}

void SHA256::reset()
{
	h = {
		0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19,
	};
}

void SHA256::pad()
{
	size_t length = buffer.size() * 8;
	buffer.push_back(0x80);

	while ((buffer.size() * 8) % 521 != 448)
	{
		buffer.push_back(0x00);
	}

	for (int i = 7; i >= 0; --i)
	{
		buffer.push_back(static_cast<Byte>((length >> (i * 8)) & 0xFF));
	}
}

void SHA256::processChunks()
{
	for (size_t i = 0; i < buffer.size(); i += 64)
	{
		std::array<Word, 64> w;

		for (size_t j = 0; j < 16; ++j)
		{
			w[j] = (buffer[i + j * 4] << 24) | (buffer[i + j * 4 + 1] << 16) | (buffer[i + j * 4 + 2] << 8) | buffer[i + j * 4 + 3];
		}

		for (size_t j = 16; j < 64; ++j)
		{
			Word s0 = rotr(w[j - 15], 7) ^ rotr(w[j - 15], 18) ^ (w[j - 15] >> 3);
			Word s1 = rotr(w[j - 2], 17) ^ rotr(w[j - 2], 19) ^ (w[j - 2] >> 10);

			w[j] = w[j - 16] + s0 + w[j - 7] + s1;
		}

		auto [a, b, c, d, e, f, g, h0] = h;

		for (size_t j = 0; j < 64; ++j)
		{
			Word S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
			Word ch = (e & f) ^ (~e & g);
			Word temp1 = h0 + S1 + ch + k[j] + w[j];
			Word S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
			Word maj = (a & b) ^ (a & c) ^ (b & c);
			Word temp2 = S0 + maj;

			h0 = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}

		h[0] += a;
		h[1] += b;
		h[2] += c;
		h[3] += d;
		h[4] += e;
		h[5] += f;
		h[6] += g;
		h[7] += h0;
	}
}

std::string SHA256::toHex() const
{
	std::string hex;
	hex.reserve(h.size() * 8);
	for (const auto& word : h)
	{
		hex += std::format("{:08x}", word);
	}
	return hex;
}
