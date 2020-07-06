#include "Map.h"

Map::Map(size_t width, size_t height, Tile defaultTile)
	: m_Tiles(new TileInstance[width * height]), m_Width(width), m_Height(height)
{
	for (int i = 0; i < width * height; i++)
	{
		m_Tiles[i].Type = defaultTile;
	}
}

Map::~Map()
{
	delete[] m_Tiles;
}

size_t Map::GetWidth() const
{
	return m_Width;
}

size_t Map::GetHeight() const
{
	return m_Height;
}

size_t Map::GetSpawnX() const
{
	return m_SpawnX;
}

size_t Map::GetSpawnY() const
{
	return m_SpawnY;
}

void Map::SetSpawn(size_t x, size_t y)
{
	this->m_SpawnX = x;
	this->m_SpawnY = y;
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
	switch (result)
	{
	case MovementResult::Free: {
		ent->Move(dx, dy);
	} break;
	case MovementResult::BlockedByTile: {
		// Entity bumped into tile.
		SetTile(nx, ny, Tile::Floor);
	} break;
	case MovementResult::BlockedByEntity: {
		// Entity bumped into another entity.
		EntityPtr other = GetEntityAt(nx, ny);
		ent->CollidedWithEntity(other);
	} break;
	default: {
		// Unhandled case, perhaps throw a warning.
	} break;
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

std::shared_ptr<Map> Map::New(size_t width, size_t height, Tile defaultTile)
{
	return std::make_shared<Map>(width, height, defaultTile);
}
