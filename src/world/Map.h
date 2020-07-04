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
private:
	TileInstance* m_Tiles;
	std::vector<EntityPtr> m_Entities;

	int m_Width;
	int m_Height;
public:
	Map(int width, int height);
	~Map();
	
	int GetWidth() const;
	int GetHeight() const;

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
private:
	void CreateRoom(const Rectangle& room);
	void CreateHTunnel(int x1, int x2, int y);
	void CreateVTunnel(int y1, int y2, int x);
};

