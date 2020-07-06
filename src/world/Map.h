#pragma once

#include "Tile.h"
#include "entity/Entity.h"

#include "gen/Shapes.h"

struct TileInstance
{
	Tile Type;
	bool Visible;
	bool Seen;

	TileInstance()
		: Type(Tile::Floor), Visible(false), Seen(false)
	{
	}
};

enum class MovementResult
{
	BlockedByEntity,
	BlockedByTile,
	Free
};

class Map
{
	friend class DungeonMapGenerator;
	friend class CaveMapGenerator;
private:
	TileInstance* m_Tiles;
	std::vector<EntityPtr> m_Entities;

	size_t m_Width;
	size_t m_Height;

	size_t m_SpawnX;
	size_t m_SpawnY;
public:
	Map(size_t width, size_t height, Tile defaultTile);
	~Map();
	
	size_t GetWidth() const;
	size_t GetHeight() const;

	size_t GetSpawnX() const;
	size_t GetSpawnY() const;

	void SetSpawn(size_t x, size_t y);

	TileInstance& GetTileInstance(int x, int y);
	Tile GetTile(int x, int y) const;
	void SetTile(int x, int y, Tile tile);

	void RecalculateVisibility(int x, int y, int radius);

	bool IsOpaque(int x, int y) const;

	bool IsVisible(int x, int y);
	void SetVisible(int x, int y, bool visible);

	bool IsSeen(int x, int y);
	void SetSeen(int x, int y, bool seen);

	void ClearVisibility();

	bool InBounds(int x, int y) const;

	EntityPtr GetEntityAt(int x, int y);
	EntityPtr GetEntity(int idx);
	std::vector<EntityPtr>& GetEntities();
	EntityPtr AddEntity(int x, int y, EntityPtr ent);

	void MoveEntity(int dx, int dy, EntityPtr ent);
	MovementResult TryMoveEntity(int x, int y);
public:
	static std::shared_ptr<Map> New(size_t width, size_t height, Tile defaultTile);
};
using MapPtr = std::shared_ptr<Map>;
