#pragma once

#include <utility>

#include "math/Number.hpp"

struct Point
{
	Number x, y;

	bool infinite;

	Point(Number x, Number y) : x(std::move(x)), y(std::move(y)), infinite(false) {}
	Point(Number x, Number y, bool infinite) : x(std::move(x)), y(std::move(y)), infinite(infinite) {}

	bool isInfinite() const { return infinite; }
	bool operator==(const Point& other) const
	{
		if (infinite && other.infinite)
		{
			return true;
		}
		if (infinite || other.infinite)
		{
			return false;
		}

		return x == other.x && y == other.y;
	}

	static const Point Zero;
	static const Point Identity;
};
