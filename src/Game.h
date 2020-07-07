#pragma once

#include <stack>

#include "lib/olcPixelGameEngine.h"
#include "gfx/ConsoleRenderer.h"
#include "input/ConsoleInputManager.h"

#include "util/Random.h"

class Scene;
class Game : public olc::PixelGameEngine
{
private:
	ConsoleRenderer* m_Renderer;
	ConsoleInputManager* m_Input;

	std::stack<std::shared_ptr<Scene>> m_Scenes;

	bool m_ShouldExit = false;
public:
	Game()
	{
		sAppName = "Altius";
	}

	bool OnUserCreate() override;
	bool OnUserUpdate(float dt) override;

	template<typename T>
	void PushScene();
	void PopScene();

	void Exit();
};

template<typename T>
inline void Game::PushScene()
{
	auto scene = std::make_shared<T>();
	scene->m_Game = this;
	scene->m_Renderer = m_Renderer;
	scene->m_Input = m_Input;
	scene->OnCreate();
	m_Scenes.push(scene);
}
