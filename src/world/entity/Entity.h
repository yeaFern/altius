#pragma once

#include "../../lib/olcPixelGameEngine.h"
#include "../../gfx/ConsoleRenderer.h"

enum class EntityType
{
	Player,
	Door,
	Goblin
};

class EntityController;
class Map;
class Entity
{
public:
	const EntityType Type;
	const std::string Name;
protected:
	EntityController* m_Controller;
	
	int m_PositionX = 0;
	int m_PositionY = 0;
	unsigned char m_Character = '@';
	olc::Pixel m_Color = olc::WHITE;
public:
	Entity(EntityType type, const std::string& name, EntityController* controller);

	void Tick(Map* map);

	void Move(int dx, int dy);

	int GetX() const;
	int GetY() const;
	unsigned char GetCharacter() const;
	olc::Pixel GetColor() const;

	virtual void Draw(ConsoleRenderer* renderer);
	virtual void CollidedWithEntity(std::shared_ptr<Entity> other);

	template<typename T>
	static std::shared_ptr<Entity> New()
	{
		return std::make_shared<T>();
	}
};
using EntityPtr = std::shared_ptr<Entity>;

class PlayerEntity : public Entity
{
public:
	PlayerEntity();

	void CollidedWithEntity(std::shared_ptr<Entity> other) override;
};

class GoblinEntity : public Entity
{
public:
	GoblinEntity();
};
