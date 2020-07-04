#pragma once

#include "../lib/olcPixelGameEngine.h"

//    name   char color            solid  transparent
#define TILE_LIST \
	T(Floor, 249, ( 35,  56,  56), true,  true ) \
	T(Wall,  '#', (107, 137, 124), false, false) \

enum class Tile
{
#define T(name, ch, color, solid, trans) name,
	TILE_LIST
#undef T
};

struct TileData
{
	unsigned char Character;
	olc::Pixel Color;
	bool Walkable;
	bool Transparent;

	TileData(unsigned char character, const olc::Pixel& color = olc::WHITE, bool walkable = true, bool transparent = false)
		: Character(character), Color(color), Walkable(walkable), Transparent(transparent)
	{
	}
};

static TileData TileDefinitions[] = {
#define T(name, ch, color, solid, trans) TileData(ch, olc::Pixel##color, solid, trans),
	TILE_LIST
#undef T
};

namespace Tiles
{
	static TileData& GetData(Tile tile)
	{
		return TileDefinitions[static_cast<int>(tile)];
	}
}
