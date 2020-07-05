#include "ConsoleInputManager.h"

ConsoleInputManager::ConsoleInputManager(olc::PixelGameEngine* engine)
	: m_Engine(engine)
{
}

void ConsoleInputManager::SetEvent(olc::Key key, std::function<void()> cb)
{
	m_Events[key] = cb;
}

void ConsoleInputManager::Update(float dt)
{
	if (m_CurrentKey == olc::Key::NONE)
	{
		for (int k = olc::Key::A; k <= olc::Key::PERIOD; k++)
		{
			olc::Key key = static_cast<olc::Key>(k);

			if (m_Engine->GetKey(key).bPressed || m_Engine->GetKey(key).bHeld)
			{
				m_CurrentKey = key;
				m_CurrentKeyState.Pressed = true;
				m_CurrentKeyState.StartTimer = 0.0f;
				m_CurrentKeyState.Timer = 0.0f;

				EmitEvent(m_CurrentKey);
			}
		}
	}
	else
	{
		m_CurrentKeyState.Pressed = false;

		m_CurrentKeyState.StartTimer += dt;
		if (m_CurrentKeyState.StartTimer > Delay)
		{
			m_CurrentKeyState.Timer += dt;
		}

		if (m_Engine->GetKey(m_CurrentKey).bHeld)
		{
			if (m_CurrentKeyState.StartTimer > Delay)
			{
				if (m_CurrentKeyState.Timer > Rate)
				{
					m_CurrentKeyState.Pressed = true;
					m_CurrentKeyState.Timer -= Rate;

					EmitEvent(m_CurrentKey);
				}
			}
		}

		if (m_Engine->GetKey(m_CurrentKey).bReleased)
		{
			m_CurrentKey = olc::Key::NONE;
		}
	}
}

olc::Key ConsoleInputManager::GetKey()
{
	if (m_CurrentKey != olc::Key::NONE)
	{
		return m_CurrentKeyState.Pressed ? m_CurrentKey : olc::Key::NONE;
	}
	else
	{
		return olc::Key::NONE;
	}
}

void ConsoleInputManager::EmitEvent(olc::Key key)
{
	auto it = m_Events.find(key);
	if (it != m_Events.end())
	{
		auto event = *it;
		event.second();
	}
}
