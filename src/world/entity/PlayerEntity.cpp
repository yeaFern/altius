#include "Entity.h"
#include "../../log/EventLog.h"

PlayerEntity::PlayerEntity()
	: Entity(EntityType::Player, "player", nullptr)
{
}

void PlayerEntity::CollidedWithEntity(std::shared_ptr<Entity> other)
{
	EventLog::Log("you bonk the " + other->Name);
}
