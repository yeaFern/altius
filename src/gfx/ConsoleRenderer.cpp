#include "ConsoleRenderer.h"

ConsoleRenderer::ConsoleRenderer(olc::PixelGameEngine* engine)
	: m_Engine(engine)
{
	this->m_FontSheet = new olc::Sprite(FontFile);
	this->m_Decal = new olc::Decal(m_FontSheet);
	this->m_CharacterWidth = m_FontSheet->width / 16;
	this->m_CharacterHeight = m_FontSheet->height / 16;
}

void ConsoleRenderer::Draw(unsigned char c, float x, float y, const olc::Pixel& fg, const olc::Pixel& bg)
{
	int xo = (int(c) % 16) * m_CharacterWidth;
	int yo = (int(c) / 16) * m_CharacterHeight;

	m_Engine->SetPixelMode([fg, bg](const int x, const int y, const olc::Pixel& src, const olc::Pixel& dest) -> olc::Pixel {
		if (src.g == 255)
		{
			return fg;
		}
		else
		{
			if (bg.a == 0)
			{
				return dest;
			}
			else
			{
				return bg;
			}
		}
	});

	m_Engine->DrawPartialSprite(x * m_CharacterWidth, y * m_CharacterHeight, m_FontSheet, xo, yo, m_CharacterWidth, m_CharacterHeight);
	//m_Engine->DrawPartialDecal({ x * float(m_CharacterWidth), y * float(m_CharacterHeight) }, m_Decal, { float(xo), float(yo) }, { float(m_CharacterWidth), float(m_CharacterHeight) });
}

void ConsoleRenderer::Draw(const std::string& s, float x, float y, const olc::Pixel& fg, const olc::Pixel& bg)
{
	for (char c : s)
	{
		Draw(c, x++, y, fg, bg);
	}
}

void ConsoleRenderer::SpookyText(const std::string& s, float x, float y, float counter, olc::Pixel color)
{
	const float amplitude = 0.15f;

	int i = 0;
	for (char c : s)
	{
		float ny = y + std::sin(counter * 6.0f + i) * amplitude;

		Draw(c, x++, ny, color, olc::Pixel(0, 0, 0, 0));

		i++;
	}
}
