#pragma once

#include "../Game.h"

class Scene
{
	friend class Game;
protected:
	Game* m_Game;
	ConsoleRenderer* m_Renderer;
	ConsoleInputManager* m_Input;
public:
    virtual void OnCreate() = 0;
    virtual void OnDestroy() = 0;
    virtual void OnUpdate(float dt) = 0;
};
using ScenePtr = std::shared_ptr<Scene>;
