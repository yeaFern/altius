#include "Game.h"

bool Game::OnUserCreate()
{
	Random::SetSeed(time(NULL));

	this->m_Renderer = new ConsoleRenderer(this);
	this->m_Input = new ConsoleInputManager(this);

	// Set up input events.
	this->m_Input->SetEvent(olc::LEFT, [&]() { MovePlayer(-1, 0); });
	this->m_Input->SetEvent(olc::RIGHT, [&]() { MovePlayer(1, 0); });
	this->m_Input->SetEvent(olc::UP, [&]() { MovePlayer(0, -1); });
	this->m_Input->SetEvent(olc::DOWN, [&]() { MovePlayer(0, 1); });
	this->m_Input->SetEvent(olc::W, [&]() {
		EventLog::Log("you wait");
		m_DoWorldTick = true;
	});

	auto generator = new DungeonMapGenerator;
	this->m_Map = generator->Generate(105, 75);
	delete generator;

	this->m_Player = m_Map->AddEntity(m_Map->GetSpawnX(), m_Map->GetSpawnY(), Entity::New<PlayerEntity>());
	m_Map->RecalculateVisibility(m_Player->GetX(), m_Player->GetY(), 12);

	return true;
}

bool Game::OnUserUpdate(float dt)
{
	// Update.
	m_EngineTimer += dt;
	m_Input->Update(dt);
	m_Renderer->UpdateTimer(dt);

	// Tick the world if we have to.
	if (m_DoWorldTick)
	{
		TickWorld();
		m_DoWorldTick = false;
	}

	// Render.
	Clear(olc::BLACK);

#if 0
	m_Renderer->Draw("--==+", 60.5, 30, olc::Pixel(50, 50, 50));
	m_Renderer->Draw("+==--", 78.5, 30, olc::Pixel(50, 50, 50));
	m_Renderer->SpookyText("Dungeon Game", 66, 30.4, m_EngineTimer, olc::Pixel::FromHSV(m_EngineTimer * 90.0f, 0.5, 0.2));
	m_Renderer->SpookyText("Dungeon Game", 66, 30.3, m_EngineTimer, olc::Pixel::FromHSV(m_EngineTimer * 90.0f, 0.5, 0.4));
	m_Renderer->SpookyText("Dungeon Game", 66, 30.2, m_EngineTimer, olc::Pixel::FromHSV(m_EngineTimer * 90.0f, 0.5, 0.6));
	m_Renderer->SpookyText("Dungeon Game", 66, 30.1, m_EngineTimer, olc::Pixel::FromHSV(m_EngineTimer * 90.0f, 0.5, 0.8));
	m_Renderer->SpookyText("Dungeon Game", 66, 30.0, m_EngineTimer, olc::Pixel::FromHSV(m_EngineTimer * 90.0f, 0.5, 1.0));

	m_Renderer->SpookyText("_______ ____", 66, 30.6, m_EngineTimer, olc::Pixel::FromHSV(m_EngineTimer * 90.0f, 0.5, 0.2));

	m_Renderer->Draw("OneLoneCoder.com - Pixel Game Engine", 0, 74, olc::Pixel(33, 33, 33));
	m_Renderer->Draw("v0.0.1", 139, 74, olc::Pixel(33, 33, 33));

	auto styled = StyledString("world", Style(olc::GREEN, olc::BLACK, StyleFX::Wavy));
	m_Renderer->Text("Hello, " + styled + "!", 1, 1);

	// Skip world stuff for now.
	return true;
#endif

	// Draw the map.
	for (int x = 0; x < m_Map->GetWidth(); x++)
	{
		for (int y = 0; y < m_Map->GetHeight(); y++)
		{
			TileData& data = Tiles::GetData(m_Map->GetTile(x, y));

			if (m_Map->IsVisible(x, y))
			{
				// If the tile is visible, draw it normally.
				m_Renderer->Draw(data.Character, x, y, data.Color);
			}
			else if (m_Map->IsSeen(x, y))
			{
				// If the tile is not visible, but has been seen, draw it darker.
				m_Renderer->Draw(data.Character, x, y, olc::Pixel(20, 33, 33));
			}
			else
			{
				// If the tile is unknown, draw some void stuff there.
				int hash = x + y * m_Map->GetWidth();
				srand(hash);
				if (rand() % 1000 < 2)
				{
					m_Renderer->Draw('\'', x, y, olc::Pixel(20, 33, 33));
				}
				else if (rand() % 1000 < 3)
				{
					m_Renderer->Draw('.', x, y, olc::Pixel(20, 33, 33));
				}
				else if (rand() % 1000 < 4)
				{
					m_Renderer->Draw(',', x, y, olc::Pixel(20, 33, 33));
				}
			}
		}
	}

	// Draw each entity.
	for (const auto& entity : m_Map->GetEntities())
	{
		// But only if it is visible.
		if (m_Map->IsVisible(entity->GetX(), entity->GetY()))
		{
			entity->Draw(m_Renderer);
		}
	}

	// Draw right UI.
	int W = 40;

	for (int i = 0; i < 75; i++)
	{
		m_Renderer->Draw(179, 145 - W, i, olc::Pixel(33, 33, 33));
		m_Renderer->Draw(179, 145 - 1, i, olc::Pixel(33, 33, 33));
	}

	// Draw visible entities to the UI.
	{
		m_Renderer->Draw("____________", 145 - W + 2, 1.2, olc::DARK_GREY);
		m_Renderer->Draw("surroundings", 145 - W + 2, 1, olc::WHITE, olc::Pixel(0, 0, 0, 0));
		int y = 3;
		for (const auto& entity : m_Map->GetEntities())
		{
			if (entity != m_Player && m_Map->IsVisible(entity->GetX(), entity->GetY()))
			{
				m_Renderer->Draw(entity->GetCharacter(), 145 - W + 2, y, entity->GetColor());
				m_Renderer->Draw(':', 145 - W + 3, y, olc::Pixel(33, 33, 33));
				m_Renderer->Draw(entity->Name, 145 - W + 4, y, olc::WHITE);

				y += 2;
			}
		}

		if (y == 3)
		{
			m_Renderer->Draw("nothing", 145 - W + 2, y, olc::WHITE);
		}
	}

	// Draw log messages.
	{
		m_Renderer->Draw("_________", 145 - W + 2, 23.2, olc::DARK_GREY);
		m_Renderer->Draw("event log", 145 - W + 2, 23.0, olc::WHITE, olc::Pixel(0, 0, 0, 0));

		int y = 25;
		for (auto it = EventLog::Instance().GetEntries().rbegin(); it != EventLog::Instance().GetEntries().rend(); it++)
		{
			m_Renderer->Draw((*it).Message, 145 - W + 2, y, olc::WHITE);
			y++;
		}
	}

	return true;
}

void Game::TickWorld()
{
	for (const auto& ent : m_Map->GetEntities())
	{
		ent->Tick(m_Map);
	}
}

void Game::MovePlayer(int dx, int dy)
{
	// Move the player entity.
	m_Map->MoveEntity(dx, dy, m_Player);

	// Recalculate the maps visibility based on the new location.
	m_Map->RecalculateVisibility(m_Player->GetX(), m_Player->GetY(), 12);

	// Since the player moved, tick the world.
	m_DoWorldTick = true;
}
