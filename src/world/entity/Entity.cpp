#include "Entity.h"
#include "ai/EntityController.h"
#include "../Map.h"

Entity::Entity(EntityType type, const std::string& name, EntityController* controller)
	: Type(type), Name(name), m_Controller(controller)
{
}

void Entity::Tick(std::shared_ptr<Map> map)
{
	if (m_Controller != nullptr)
	{
		m_Controller->Tick(map, this);
	}
}

void Entity::Move(int dx, int dy)
{
	m_PositionX += dx;
	m_PositionY += dy;
}

int Entity::GetX() const
{
	return m_PositionX;
}

int Entity::GetY() const
{
	return m_PositionY;
}

unsigned char Entity::GetCharacter() const
{
	return m_Character;
}

olc::Pixel Entity::GetColor() const
{
	return m_Color;
}

void Entity::Draw(ConsoleRenderer* renderer)
{
	renderer->Draw(m_Character, m_PositionX, m_PositionY, m_Color);
}

void Entity::CollidedWithEntity(std::shared_ptr<Entity> other)
{
}
