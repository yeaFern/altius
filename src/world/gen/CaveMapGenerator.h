#pragma once

#include "MapGenerator.h"
#include "../../util/Random.h"

class CaveMapGenerator : public MapGenerator
{
public:
	virtual MapPtr Generate(size_t width, size_t height) override;
};
