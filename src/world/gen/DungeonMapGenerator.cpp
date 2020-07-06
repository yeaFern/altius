#include "DungeonMapGenerator.h"

static constexpr int RoomCount = 30;
static constexpr int MinRoomSize = 6;
static constexpr int MaxRoomSize = 10;
static constexpr int MaxMonsters = 3;

void DungeonMapGenerator::CreateHTunnel(MapPtr map, int x1, int x2, int y)
{
	if (x2 < x1) std::swap(x1, x2);

	for (int x = x1; x < x2 + 1; x++)
	{
		map->m_Tiles[x + y * map->m_Width].Type = Tile::Floor;
	}
}

void DungeonMapGenerator::CreateVTunnel(MapPtr map, int y1, int y2, int x)
{
	if (y2 < y1) std::swap(y1, y2);

	for (int y = y1; y < y2 + 1; y++)
	{
		map->m_Tiles[x + y * map->m_Width].Type = Tile::Floor;
	}
}

void DungeonMapGenerator::CreateRoom(MapPtr map, const Rectangle& room)
{
	for (int x = room.X1 + 1; x < room.X2; x++)
	{
		for (int y = room.Y1 + 1; y < room.Y2; y++)
		{
			map->m_Tiles[x + y * map->m_Width].Type = Tile::Floor;
		}
	}
}

MapPtr DungeonMapGenerator::Generate(size_t width, size_t height)
{
	MapPtr map = Map::New(width, height, Tile::Wall);

	std::vector<Rectangle> rooms;

	// Attempt to generate some amount of rooms.
	for (int i = 0; i < RoomCount; i++)
	{
		// Randomly choose room size and location.
		int w = Random::Range(MinRoomSize, MaxRoomSize);
		int h = Random::Range(MinRoomSize, MaxRoomSize);
		int x = Random::Range(0, width - w - 1);
		int y = Random::Range(0, height - h - 1);

		Rectangle newRoom(x, y, w, h);

		// Ensure that this position is valid for a room.
		bool badPosition = false;
		for (const auto& other : rooms)
		{
			int x1;
			int y1;
			newRoom.GetCenter(x1, y1);

			int x2;
			int y2;
			other.GetCenter(x2, y2);

			// Disallow if the room is too close to any other.
			if (std::sqrt(
				((x2 - x1) * (x2 - x1)) +
				((y2 - y1) * (y2 - y1))
			) < MaxRoomSize + 1)
			{
				badPosition = true;
				break;
			}
		}

		if (!badPosition)
		{
			// Spawn the room in the world.
			CreateRoom(map, newRoom);

			// Connect every room with the previous room.
			if (rooms.size() > 0)
			{
				int px;
				int py;
				rooms[rooms.size() - 1].GetCenter(px, py);

				int nx;
				int ny;
				newRoom.GetCenter(nx, ny);

				if (Random::Float() < 0.5f)
				{
					CreateHTunnel(map, px, nx, py);
					CreateVTunnel(map, py, ny, nx);
				}
				else
				{
					CreateVTunnel(map, py, ny, px);
					CreateHTunnel(map, px, nx, ny);
				}
			}

			rooms.push_back(newRoom);
		}
	}

	int spawnX;
	int spawnY;
	rooms[0].GetCenter(spawnX, spawnY);
	map->SetSpawn(spawnX, spawnY);

	// Randomly add some goblins.
	for (const auto& room : rooms)
	{
		int x = Random::Range(room.X1 + 1, room.X2);
		int y = Random::Range(room.Y1 + 1, room.Y2);

		if (Random::Float() < 0.8f)
		{
			map->AddEntity(x, y, Entity::New<GoblinEntity>());
		}
	}

	return map;
}
