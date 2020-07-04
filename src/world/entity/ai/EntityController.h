#pragma once

#include "../Entity.h"
#include "../Map.h"
#include "../../../log/EventLog.h"

class EntityController
{
public:
	virtual ~EntityController() = default;

	virtual void Tick(Map* map, Entity* entity) = 0;
};

class WanderController : public EntityController
{
public:
	void Tick(Map* map, Entity* entity) override
	{
		srand(time(NULL));
		if(rand() % 100 < 80)
		{
			int dx = (rand() % 100 < 50) ? 0 : (rand() % 100) < 50 ? -1 : 1;
			int dy = (rand() % 100 < 50) ? 0 : (rand() % 100) < 50 ? -1 : 1;

			if (map->TryMoveEntity(entity->GetX() + dx, entity->GetY() + dy) == MovementResult::Free)
			{
				entity->Move(dx, dy);

				if (map->IsVisible(entity->GetX(), entity->GetY()))
				{
					EventLog::Log(entity->Name + " wanders about");
				}
			}
		}
	}
};
