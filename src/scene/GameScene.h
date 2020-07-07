#pragma once

#include "Scene.h"

#include "../log/EventLog.h"

#include "../world/entity/Entity.h"
#include "../world/gen/CaveMapGenerator.h"
#include "../world/gen/DungeonMapGenerator.h"

class GameScene : public Scene
{
private:
	bool m_DoWorldTick = false;
	MapPtr m_Map;
	EntityPtr m_Player;
public:
	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float dt) override;
private:
	void TickWorld();

	void MovePlayer(int dx, int dy);
};
