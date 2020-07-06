#pragma once

#include "../Map.h"

class MapGenerator
{
public:
	virtual MapPtr Generate(size_t width, size_t height) = 0;
};
