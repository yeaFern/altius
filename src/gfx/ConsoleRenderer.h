#pragma once

#include "../lib/olcPixelGameEngine.h"

#include "StyledString.h"

class ConsoleRenderer
{
public:
	static constexpr auto FontFile = "assets/8x8_thick.png";
private:
	olc::PixelGameEngine* m_Engine;
	olc::Sprite* m_FontSheet;
	olc::Decal* m_Decal;

	int m_CharacterWidth;
	int m_CharacterHeight;

	float m_Timer;
public:
	ConsoleRenderer(olc::PixelGameEngine* engine);

	void UpdateTimer(float dt);

	void Draw(unsigned char c, float x, float y, const olc::Pixel& fg, const olc::Pixel& bg = olc::BLACK);
	void Draw(const std::string& s, float x, float y, const olc::Pixel& fg, const olc::Pixel& bg = olc::BLACK);

	void SpookyText(const std::string& s, float x, float y, float counter, olc::Pixel color);

	void Text(const StyledString& str, float x, float y);
};
