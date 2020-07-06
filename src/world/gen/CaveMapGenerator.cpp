#include "CaveMapGenerator.h"

static constexpr auto InitialChance = 0.38f;
static constexpr auto DeathLimit = 3;
static constexpr auto BirthLimit = 4;
static constexpr auto Iterations = 8;

MapPtr CaveMapGenerator::Generate(size_t width, size_t height)
{
	MapPtr map = Map::New(width, height, Tile::Wall);

	// Randomise the maps tiles.
	for (int i = 0; i < width * height; i++)
	{
		map->m_Tiles[i].Type = Random::Float() < InitialChance ? Tile::Wall : Tile::Floor;
	}

	// Counts the neighbours of the given tile.
	auto CountNeighbours = [&](int x, int y) -> int {
		int result = 0;
		for (int yo = -1; yo <= 1; yo++)
		{
			for (int xo = -1; xo <= 1; xo++)
			{
				if (yo == 0 && xo == 0)
				{
					continue;
				}

				int nx = x + xo;
				int ny = y + yo;

				if (nx < 0 || nx >= width || ny < 0 || ny >= height)
				{
					result++;
				}
				else if (map->m_Tiles[nx + ny * width].Type == Tile::Wall)
				{
					result++;
				}
			}
		}
		return result;
	};

	// Erode the map some amount of times.
	for (int i = 0; i < Iterations; i++)
	{
		TileInstance* newTiles = new TileInstance[width * height];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int neighbours = CountNeighbours(x, y);
				int idx = x + y * width;

				if (map->m_Tiles[idx].Type == Tile::Wall)
				{
					newTiles[idx].Type = neighbours >= DeathLimit ? Tile::Wall : Tile::Floor;
				}
				else
				{
					newTiles[idx].Type = neighbours > BirthLimit ? Tile::Wall : Tile::Floor;
				}
			}
		}
		delete[] map->m_Tiles;
		map->m_Tiles = newTiles;
	}

	// Fill in the edges so there are no gaps.
	for (int i = 0; i < width; i++)
	{
		map->m_Tiles[i + 0 * width].Type = Tile::Wall;
		map->m_Tiles[i + (height - 1) * width].Type = Tile::Wall;
	}

	for (int i = 0; i < height; i++)
	{
		map->m_Tiles[0 + i * width].Type = Tile::Wall;
		map->m_Tiles[(width - 1) + i * width].Type = Tile::Wall;
	}

	// TODO: Select a proper spawn point.
	map->SetSpawn(20, 20);

	return map;
}
