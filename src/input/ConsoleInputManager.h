#pragma once

#include <unordered_map>
#include <functional>

#include "../lib/olcPixelGameEngine.h"

class ConsoleInputManager
{
public:
	static constexpr auto Delay = 300.0f / 1000.0f;
	static constexpr auto Rate = 60.0f / 1000.0f;
private:
	struct KeyState
	{
		float StartTimer = 0.0f;
		float Timer = 0.0f;
		bool Pressed = false;
	};
private:
	olc::PixelGameEngine* m_Engine;

	olc::Key m_CurrentKey = olc::Key::NONE;
	KeyState m_CurrentKeyState;

	std::unordered_map<olc::Key, std::function<void()>> m_Events;
public:
	ConsoleInputManager(olc::PixelGameEngine* engine);

	void SetEvent(olc::Key key, std::function<void()> cb);

	void Update(float dt);

	olc::Key GetKey();
private:
	void EmitEvent(olc::Key key);
};
