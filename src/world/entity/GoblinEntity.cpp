#include "Entity.h"
#include "ai/EntityController.h"

GoblinEntity::GoblinEntity()
	: Entity(EntityType::Goblin, "goblin", new WanderController)
{
	this->m_Character = 'g';
	this->m_Color = olc::Pixel(40, 122, 37);
}
