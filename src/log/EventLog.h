#pragma once

#include <string>
#include <vector>

struct LogEntry
{
	std::string Message;

	LogEntry(const std::string& message)
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

	static void Log(const std::string& message)
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
	void AddEntry(const std::string& message);

	std::vector<LogEntry>& GetEntries();
};
