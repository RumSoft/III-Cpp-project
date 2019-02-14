#pragma once

#include <map>
#include <vector>
#include <CryString/CryString.h>
#include <ctime>
#include <queue>

class Logger
{
	Logger() = default;
	std::map<string, std::map<int, string>> _logs;
public:
	static Logger& Get()
	{
		static auto* instance = new Logger();
		return *instance;
	}

	void Log(const string& source, const string& text, const int line = 0)
	{
		_logs[source][line] = text;
	}

	string ReadLog()
	{
		string str;
		for (auto& source : _logs)
			for (auto& line : source.second)
			{
				str.append(source.first);
				str.append(": ");
				str.append(line.second);
				str.append("\n");
			}
		return str;
	}
};

class Snackbar
{
	struct SnackbarElement
	{
		string text;
		float expire;
	};
	Snackbar() = default;

	std::vector<SnackbarElement> _logs;
public:
	static Snackbar& Get()
	{
		static auto* instance = new Snackbar();
		return *instance;
	}

	void Log(const string& source, const float duration = 1)
	{
		_logs.push_back({ source, clock() + duration * CLOCKS_PER_SEC });
	}

	string ReadLog()
	{
		const auto now = clock();
		string str;
		for (int i = _logs.size() - 1; i >= 0; i--)
			if (_logs[i].expire < now)
				_logs.erase(_logs.begin() + i);
		for (int i = _logs.size() - 1; i >= 0; i--)
			str.AppendFormat("%s\n", _logs[i].text);
		return str;
	}
};