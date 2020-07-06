#pragma once

#include "MapGenerator.h"
#include "../../util/Random.h"

class DungeonMapGenerator : public MapGenerator
{
private:
	void CreateHTunnel(MapPtr map, int x1, int x2, int y);
	void CreateVTunnel(MapPtr map, int y1, int y2, int x);
	void CreateRoom(MapPtr map, const Rectangle& room);
public:
	virtual MapPtr Generate(size_t width, size_t height) override;
};
