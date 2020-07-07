#include "CreditsScene.h"

void CreditsScene::OnCreate()
{
}

void CreditsScene::OnDestroy()
{
}

void CreditsScene::OnUpdate(float dt)
{
	std::array<StyledString, 2> lines = {
		"Made with " + StyledString("olcPixelGameEngine", Style(olc::Pixel::FromHSV(316, 0.78, 0.88), olc::BLACK, StyleFX::Wavy)),
		"Copyright 2018 - 2020 OneLoneCoder.com"
	};

	for (int i = 0; i < lines.size(); i++)
	{
		float x = (145.0f / 2.0f) - (float(lines[i].GetString().size()) / 2.0f);
		float y = 8 + 1 + 7 + 12 + (i * 2);

		m_Renderer->Text(lines[i], x, y);
	}

	if (m_Input->GetKey() == olc::ESCAPE)
	{
		m_Game->PopScene();
	}
}
