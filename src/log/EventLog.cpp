#include "EventLog.h"

void EventLog::AddEntry(const StyledString& message)
{
	m_Entries.emplace_back(message);
}

std::vector<LogEntry>& EventLog::GetEntries()
{
	return m_Entries;
}
