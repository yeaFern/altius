#include "GameScene.h"

void GameScene::OnCreate()
{
	// Set up input events.
	this->m_Input->SetEvent(olc::LEFT, [&]() { MovePlayer(-1, 0); });
	this->m_Input->SetEvent(olc::RIGHT, [&]() { MovePlayer(1, 0); });
	this->m_Input->SetEvent(olc::UP, [&]() { MovePlayer(0, -1); });
	this->m_Input->SetEvent(olc::DOWN, [&]() { MovePlayer(0, 1); });
	this->m_Input->SetEvent(olc::W, [&]() {
		EventLog::Log("you wait");
		m_DoWorldTick = true;
	});
	this->m_Input->SetEvent(olc::ESCAPE, [&]() { m_Game->PopScene(); });

	auto generator = new DungeonMapGenerator;
	this->m_Map = generator->Generate(105, 75);
	delete generator;

	this->m_Player = m_Map->AddEntity(m_Map->GetSpawnX(), m_Map->GetSpawnY(), Entity::New<PlayerEntity>());
	m_Map->RecalculateVisibility(m_Player->GetX(), m_Player->GetY(), 12);
}

void GameScene::OnDestroy()
{
	// Remove input events.
	this->m_Input->SetEvent(olc::LEFT, nullptr);
	this->m_Input->SetEvent(olc::RIGHT, nullptr);
	this->m_Input->SetEvent(olc::UP, nullptr);
	this->m_Input->SetEvent(olc::DOWN, nullptr);
	this->m_Input->SetEvent(olc::W, nullptr);
	this->m_Input->SetEvent(olc::ESCAPE, nullptr);
}

void GameScene::OnUpdate(float dt)
{
	// Tick the world if we have to.
	if (m_DoWorldTick)
	{
		TickWorld();
		m_DoWorldTick = false;
	}

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

}

void GameScene::TickWorld()
{
	for (const auto& ent : m_Map->GetEntities())
	{
		ent->Tick(m_Map);
	}
}

void GameScene::MovePlayer(int dx, int dy)
{
	// Move the player entity.
	m_Map->MoveEntity(dx, dy, m_Player);

	// Recalculate the maps visibility based on the new location.
	m_Map->RecalculateVisibility(m_Player->GetX(), m_Player->GetY(), 12);

	// Since the player moved, tick the world.
	m_DoWorldTick = true;
}
