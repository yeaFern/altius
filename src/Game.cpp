#include "Game.h"
#include "scene/Scene.h"
#include "scene/MenuScene.h"

bool Game::OnUserCreate()
{
	Random::SetSeed(time(NULL));

	this->m_Renderer = new ConsoleRenderer(this);
	this->m_Input = new ConsoleInputManager(this);

	PushScene<MenuScene>();

	return true;
}

bool Game::OnUserUpdate(float dt)
{
	// Update.
	m_Input->Update(dt);
	m_Renderer->UpdateTimer(dt);

	// Render.
	Clear(olc::BLACK);

	if (!m_Scenes.empty())
	{
		m_Scenes.top()->OnUpdate(dt);
	}

	return !m_ShouldExit;
}

void Game::PopScene()
{
	if (!m_Scenes.empty())
	{
		auto scene = m_Scenes.top();
		scene->OnDestroy();
		m_Scenes.pop();
	}
}

void Game::Exit()
{
	this->m_ShouldExit = true;
}
