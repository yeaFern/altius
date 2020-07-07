#pragma once

#include <string>
#include <vector>

#include "../gfx/StyledString.h"

struct LogEntry
{
	StyledString Message;

	LogEntry(const StyledString& message)
		: Message(message)
	{
	}
};

class EventLog
{
public:
	static EventLog& Instance()
	{
		static EventLog instance;
		return instance;
	}

	static void Log(const StyledString& message)
	{
		Instance().AddEntry(message);
	}
private:
	EventLog() {}
public:
	EventLog(EventLog const&) = delete;
	void operator=(EventLog const&) = delete;
private:
	std::vector<LogEntry> m_Entries;
public:
	void AddEntry(const StyledString& message);

	std::vector<LogEntry>& GetEntries();
};
