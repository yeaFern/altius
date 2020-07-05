#pragma once

#include <string>
#include <set>

#include "../lib/olcPixelGameEngine.h"

enum class StyleFX
{
	Standard,
	Wavy
};

struct Style
{
	size_t Position;
	olc::Pixel Foreground;
	olc::Pixel Background;
	StyleFX Effects;

	Style(const olc::Pixel& fg = olc::WHITE, const olc::Pixel& bg = olc::BLACK, StyleFX effects = StyleFX::Standard)
		: Position(0), Foreground(fg), Background(bg), Effects(effects)
	{
	}

	bool operator< (const Style& rhs) const { return Position < rhs.Position; }
};

class StyledString
{
private:
	std::string m_Contents;
	std::set<Style> m_Style;
public:
	StyledString(const std::string& str, const Style& = Style());
	StyledString(const char* str, const Style& style = Style());
	StyledString(char chr, const Style& style = Style());

	void AddStyle(size_t position, const Style& style);
	void AppendStyles(const std::set<Style>& styles);

	std::string GetString() const;
	Style GetStyleAt(size_t position) const;

	StyledString operator+(const StyledString& rhs);
	friend StyledString operator+(const char* lhs, const StyledString& rhs);
};
