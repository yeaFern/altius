#include "Map.h"

static constexpr auto InitialChance = 0.38f;
static constexpr auto DeathLimit = 3;
static constexpr auto BirthLimit = 4;
static constexpr auto Iterations = 4;

Map::Map(int width, int height)
	: m_Tiles(new TileInstance[width * height]), m_Width(width), m_Height(height)
{
	auto RandomFloat = []()
	{
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	};

#if 0
	for (int i = 0; i < width * height; i++)
	{
		m_Tiles[i].Type = RandomFloat() < InitialChance ? Tile::Wall : Tile::Floor;
	}

	for (int i = 0; i < 8; i++)
	{
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

					if (nx < 0 || nx >= m_Width || ny < 0 || ny >= m_Height)
					{
						result++;
					}
					else if (m_Tiles[nx + ny * m_Width].Type == Tile::Wall)
					{
						result++;
					}
				}
			}
			return result;
		};

		TileInstance* newTiles = new TileInstance[m_Width * m_Height];
		for (int y = 0; y < m_Height; y++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				int neighbours = CountNeighbours(x, y);
				int idx = x + y * m_Width;

				if (m_Tiles[idx].Type == Tile::Wall)
				{
					newTiles[idx].Type = neighbours >= DeathLimit ? Tile::Wall : Tile::Floor;
				}
				else
				{
					newTiles[idx].Type = neighbours > BirthLimit ? Tile::Wall : Tile::Floor;
				}
			}
		}
		delete[] m_Tiles;
		m_Tiles = newTiles;
	}

	for (int i = 0; i < m_Width; i++)
	{
		m_Tiles[i + 0 * m_Width].Type = Tile::Wall;
		m_Tiles[i + (m_Height - 1) * m_Width].Type = Tile::Wall;
	}

	for (int i = 0; i < m_Height; i++)
	{
		m_Tiles[0 + i * m_Width].Type = Tile::Wall;
		m_Tiles[(m_Width - 1) + i * m_Width].Type = Tile::Wall;
	}

#else
	for (int i = 0; i < width * height; i++)
	{
		m_Tiles[i].Type = Tile::Wall;
	}

	auto room1 = Rectangle(20, 15, 10, 15);
	auto room2 = Rectangle(35, 15, 10, 15);

	std::vector<Rectangle> rooms;
	int room_num = 0;

	const int RoomCount = 30;
	const int MinRoomSize = 6;
	const int MaxRoomSize = 10;

	auto randint = [](int min, int max) {
		return min + (std::rand() % (max - min + 1));
	};

	for (int i = 0; i < RoomCount; i++)
	{
		int w = randint(MinRoomSize, MaxRoomSize);
		int h = randint(MinRoomSize, MaxRoomSize);
		int x = randint(0, m_Width - w - 1);
		int y = randint(0, m_Height - h - 1);

		Rectangle newRoom(x, y, w, h);
		bool overlap = false;
		for (const auto& other : rooms)
		{
			if (newRoom.Intersects(other))
			{
				overlap = true;
				break;
			}
		}

		if (!overlap)
		{
			CreateRoom(newRoom);

			if (room_num > 0)
			{
				int px;
				int py;
				rooms[room_num - 1].GetCenter(px, py);

				int nx;
				int ny;
				newRoom.GetCenter(nx, ny);

				if (randint(0, 1) == 1)
				{
					CreateHTunnel(px, nx, py);
					CreateVTunnel(py, ny, nx);
				}
				else
				{
					CreateVTunnel(py, ny, px);
					CreateHTunnel(px, nx, ny);
				}
			}

			rooms.push_back(newRoom);
			room_num++;
		}
	}

	const int MaxMonsters = 3;
	for (const auto& room : rooms)
	{
		int x = randint(room.X1 + 1, room.X2 - 1);
		int y = randint(room.Y1 + 1, room.Y2 - 1);

		if (randint(0, 100) < 80)
		{
			AddEntity(x, y, Entity::New<GoblinEntity>());
		}
	}

	for (int i = 0; i < width * height; i++)
	{
		//m_Tiles[i].Type = Tile::Floor;
		//m_Tiles[i].Visible = true;
	}
#endif
}

Map::~Map()
{
	delete[] m_Tiles;
}

int Map::GetWidth() const
{
	return m_Width;
}

int Map::GetHeight() const
{
	return m_Height;
}

TileInstance& Map::GetTileInstance(int x, int y)
{
	if (InBounds(x, y))
	{
		return m_Tiles[x + y * m_Width];
	}
	else
	{
		return m_Tiles[0];
	}
}

Tile Map::GetTile(int x, int y) const
{
	if (!InBounds(x, y))
	{
		return Tile::Wall;
	}
	else
	{
		return m_Tiles[x + y * m_Width].Type;
	}
}

void Map::SetTile(int x, int y, Tile tile)
{
	if (InBounds(x, y))
	{
		m_Tiles[x + y * m_Width].Type = tile;
	}
}

static int multipliers[4][8] = {
	{1, 0, 0, -1, -1, 0, 0, 1},
	{0, 1, -1, 0, 0, -1, 1, 0},
	{0, 1, 1, 0, 0, -1, -1, 0},
	{1, 0, 0, 1, -1, 0, 0, -1}
};

void Map::RecalculateVisibility(int xp, int yp, int radius)
{
	ClearVisibility();

	using CastFunction = std::function<void(int, int, int, int, float, float, int, int, int, int)>;
	CastFunction Cast = [&](
		int x, int y,
		int radius,
		int row,
		float startSlope, float endSlope,
		int xx, int xy,
		int yx, int yy)
	{
		if (startSlope < endSlope)
		{
			return;
		}

		SetVisible(x, y, true);
		SetSeen(x, y, true);

		float next_start_slope = startSlope;
		for (int i = row; i <= radius; i++)
		{
			bool blocked = false;
			for (int dx = -i, dy = -i; dx <= 0; dx++)
			{
				float l_slope = (dx - 0.5) / (dy + 0.5);
				float r_slope = (dx + 0.5) / (dy - 0.5);
				if (startSlope < r_slope)
				{
					continue;
				}
				else if (endSlope > l_slope)
				{
					break;
				}

				int sax = dx * xx + dy * xy;
				int say = dx * yx + dy * yy;
				if ((sax < 0 && (int)std::abs(sax) > x) ||
					(say < 0 && (int)std::abs(say) > y))
				{
					continue;
				}
				int ax = x + sax;
				int ay = y + say;
				if (ax >= GetWidth() || ay >= GetHeight())
				{
					continue;
				}

				int radius2 = radius * radius;
				if ((int)(dx * dx + dy * dy) < radius2)
				{
					SetVisible(ax, ay, true);
					SetSeen(ax, ay, true);
				}

				if (blocked)
				{
					if (IsOpaque(ax, ay))
					{
						next_start_slope = r_slope;
						continue;
					}
					else
					{
						blocked = false;
						startSlope = next_start_slope;
					}
				}
				else if (IsOpaque(ax, ay))
				{
					blocked = true;
					next_start_slope = r_slope;
					Cast(x, y, radius, i + 1, startSlope, l_slope, xx, xy, yx, yy);
				}
			}
			if (blocked)
			{
				break;
			}
		}
	};

	for (int i = 0; i < 8; i++)
	{

		Cast(xp, yp, radius, 1, 1.0, 0.0, multipliers[0][i],
			multipliers[1][i], multipliers[2][i], multipliers[3][i]);
	}
}

bool Map::IsOpaque(int x, int y) const
{
	if (!InBounds(x, y))
	{
		return true;
	}
	return !Tiles::GetData(m_Tiles[x + y * m_Width].Type).Transparent;
}

bool Map::IsVisible(int x, int y)
{
	if (!InBounds(x, y))
	{
		return false;
	}
	return m_Tiles[x + y * m_Width].Visible;
}

void Map::SetVisible(int x, int y, bool visible)
{
	if (InBounds(x, y))
	{
		m_Tiles[x + y * m_Width].Visible = visible;
	}
}

bool Map::IsSeen(int x, int y)
{
	if (!InBounds(x, y))
	{
		return false;
	}
	return m_Tiles[x + y * m_Width].Seen;
}

void Map::SetSeen(int x, int y, bool seen)
{
	if (InBounds(x, y))
	{
		m_Tiles[x + y * m_Width].Seen = seen;
	}
}

void Map::ClearVisibility()
{
	for (int i = 0; i < m_Width * m_Height; i++)
	{
		m_Tiles[i].Visible = false;
	}
}

bool Map::InBounds(int x, int y) const
{
	return x >= 0 && y >= 0 && x < m_Width && y < m_Height;
}

EntityPtr Map::GetEntityAt(int x, int y)
{

	for (auto& ent : m_Entities)
	{
		if (ent->GetX() == x && ent->GetY() == y)
		{
			return ent;
		}
	}

	return nullptr;
}

EntityPtr Map::GetEntity(int idx)
{
	return m_Entities[idx];
}

std::vector<EntityPtr>& Map::GetEntities()
{
	return m_Entities;
}

EntityPtr Map::AddEntity(int x, int y, EntityPtr ent)
{
	int idx = m_Entities.size();
	m_Entities.push_back(ent);
	ent->Move(x, y);
	// TODO: Maybe do something with the ID?
	return ent;
}

void Map::MoveEntity(int dx, int dy, EntityPtr ent)
{
	if (dx == 0 && dy == 0) return;

	int nx = ent->GetX() + dx;
	int ny = ent->GetY() + dy;

	auto result = TryMoveEntity(nx, ny);
	if (result == MovementResult::Free)
	{
		ent->Move(dx, dy);
	}
	else if (result == MovementResult::BlockedByTile)
	{
		// Entity bumped into tile.
		SetTile(nx, ny, Tile::Floor);
	}
	else if (result == MovementResult::BlockedByEntity)
	{
		// Entity bumped into another entity.
		EntityPtr other = GetEntityAt(nx, ny);
		ent->CollidedWithEntity(other);

		//entity->Health -= 5;
		//if (entity->Health <= 0)
		//{
		//	entity->Color = olc::DARK_RED;
		//}
		//Log("Bonk the goblin");

	}
	else
	{
		// Unhandled case, perhaps throw a warning.
	}
}

MovementResult Map::TryMoveEntity(int x, int y)
{
	if (!InBounds(x, y))
	{
		return MovementResult::BlockedByTile;
	}

	if (!Tiles::GetData(GetTile(x, y)).Walkable)
	{
		return MovementResult::BlockedByTile;
	}

	if (GetEntityAt(x, y) != nullptr)
	{
		return MovementResult::BlockedByEntity;
	}

	return MovementResult::Free;
}

void Map::CreateRoom(const Rectangle& room)
{
	for (int x = room.X1 + 1; x < room.X2; x++)
	{
		for (int y = room.Y1 + 1; y < room.Y2; y++)
		{
			m_Tiles[x + y * m_Width].Type = Tile::Floor;
		}
	}
}

void Map::CreateHTunnel(int x1, int x2, int y)
{
	if (x2 < x1) std::swap(x1, x2);

	for (int x = x1; x < x2 + 1; x++)
	{
		m_Tiles[x + y * m_Width].Type = Tile::Floor;
	}
}

void Map::CreateVTunnel(int y1, int y2, int x)
{
	if (y2 < y1) std::swap(y1, y2);

	for (int y = y1; y < y2 + 1; y++)
	{
		m_Tiles[x + y * m_Width].Type = Tile::Floor;
	}
}
