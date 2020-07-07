#include "MenuScene.h"
#include "GameScene.h"

void MenuScene::OnCreate()
{
}

void MenuScene::OnDestroy()
{
}

void MenuScene::OnUpdate(float dt)
{
	std::string logo;
	logo += ".......##..#....#.............";
	logo += "........#..#..................";
	logo += ".####...#.###..##..#...#..####";
	logo += "....#...#..#....#..#...#.#....";
	logo += ".####...#..#....#..#...#..###.";
	logo += "#...#...#..#.#..#..#..##.....#";
	logo += ".#####.###.##..###..##.#.####.";
	const int LogoWidth = 30;
	const int LogoHeight = 7;

	const float ShineStart = -32.0f;
	const float ShineEnd = 128.0f;
	const float ShineSpeed = 48.0f;
	const float ShineThickness = 4.0f;

	static float M = 1.0f;
	static float C = ShineStart;

	C += ShineSpeed * dt;
	if (C >= ShineEnd)
	{
		C = ShineStart;
	}

	int xp = (145 / 2) - (LogoWidth / 2);

	static float Offset = 0.0f;
	Offset += dt;

	for (int y = 0; y < LogoHeight; y++)
	{
		for (int x = 0; x < LogoWidth; x++)
		{
			if (logo[x + y * LogoWidth] == '#')
			{
				if (std::abs((M * x + C) - y) < ShineThickness)
				{
					m_Renderer->Draw(219, xp + x, 8 + y, olc::WHITE);
				}
				else
				{
					float r = (Random::Noise((x + y * LogoWidth) + int(Offset)) * 0.2f) - 0.1f;
					m_Renderer->Draw(219, xp + x, 8 + y, olc::Pixel::FromHSV(316, 0.78 + r, 0.88));
				}
			}
		}
	}

	m_Renderer->Draw("v0.0.1", (145 / 2) - (6 / 2), 8 + 1 + LogoHeight, olc::Pixel(33, 33, 33));

	static std::array<std::string, 4> MenuItems = {
		"play",
		"options",
		"credits",
		"exit"
	};
	static int Selection = 0;

	//{
	//	float mx = GetMouseX() / 8.0f;
	//	float my = GetMouseY() / 8.0f;
	//
	//	for (int i = 0; i < MenuItems.size(); i++)
	//	{
	//		std::string item = MenuItems[i];
	//
	//		float x = (145.0f / 2.0f) - (float(item.size()) / 2.0f);
	//		float y = 8 + 1 + LogoHeight + 12 + (i * 2);
	//
	//		if (int(my) == int(y) && mx >= x && mx < x + item.size())
	//		{
	//			Selection = i;
	//		}
	//	}
	//}

	for (int i = 0; i < MenuItems.size(); i++)
	{
		std::string item = MenuItems[i];

		float x = (145.0f / 2.0f) - (float(item.size()) / 2.0f);
		float y = 8 + 1 + LogoHeight + 12 + (i * 2);

		if (Selection == i)
		{
			m_Renderer->Text(
				StyledString(item, Style(olc::WHITE, olc::BLACK, StyleFX::Wavy)),
				x, y
			);

			m_Renderer->Draw("-=>", x - 3.5f, y, olc::Pixel(33, 33, 33));
			m_Renderer->Draw("<=-", x + item.size() + 0.5f, y, olc::Pixel(33, 33, 33));
		}
		else
		{
			m_Renderer->Draw(item, x, y, olc::WHITE);
		} 
	}

	if (m_Input->GetKey() == olc::DOWN)
	{
		Selection = (Selection + 1) % MenuItems.size();
	}

	if (m_Input->GetKey() == olc::UP)
	{
		Selection = (Selection - 1) % MenuItems.size();
	}

	if (m_Input->GetKey() == olc::SPACE || m_Input->GetKey() == olc::ENTER)
	{
		if (Selection == 0)
		{
			m_Game->PushScene<GameScene>();
		}

		if (Selection == 3)
		{
			m_Game->Exit();
		}
	}

}
