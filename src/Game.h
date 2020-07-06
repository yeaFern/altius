#pragma once

#include "lib/olcPixelGameEngine.h"

#include "gfx/ConsoleRenderer.h"

#include "input/ConsoleInputManager.h"

#include "log/EventLog.h"

#include "world/entity/Entity.h"
#include "world/gen/CaveMapGenerator.h"
#include "world/gen/DungeonMapGenerator.h"

#include "util/Random.h"

class Game : public olc::PixelGameEngine
{
private:
	ConsoleRenderer* m_Renderer;
	ConsoleInputManager* m_Input;

	float m_EngineTimer = 0.0f;

	bool m_DoWorldTick = false;
	MapPtr m_Map;
	EntityPtr m_Player;
public:
	Game()
	{
		sAppName = "Dungeon Game";
	}

	bool OnUserCreate() override;
	bool OnUserUpdate(float dt) override;

	void TickWorld();

	void MovePlayer(int dx, int dy);
};
