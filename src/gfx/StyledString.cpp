#include "StyledString.h"

StyledString::StyledString(const std::string& str, const Style& style)
	: m_Contents(str)
{
	AddStyle(0, style);
}

StyledString::StyledString(const char* str, const Style& style)
	: StyledString(std::string(str), style)
{
}

StyledString::StyledString(char chr, const Style& style)
	: StyledString(std::string(chr + ""), style)
{
}

void StyledString::AddStyle(size_t position, const Style& style)
{
	Style copy = style;
	copy.Position = position;
	m_Style.insert(copy);
}

void StyledString::AppendStyles(const std::set<Style>& styles)
{
	size_t offset = m_Contents.size();
	for (auto style : styles)
	{
		style.Position += offset;
		m_Style.insert(style);
	}
}

std::string StyledString::GetString() const
{
	return m_Contents;
}

Style StyledString::GetStyleAt(size_t position) const
{
	for (auto it = m_Style.rbegin(); it != m_Style.rend(); ++it)
	{
		auto style = *it;
		if (style.Position <= position)
		{
			return style;
		}
	}

	return *m_Style.begin();
}

StyledString StyledString::operator+(const StyledString& rhs)
{
	AppendStyles(rhs.m_Style);
	m_Contents += rhs.m_Contents;
	return *this;
}

StyledString operator+(const char* lhs, const StyledString& rhs)
{
	return StyledString(lhs) + rhs;
}
