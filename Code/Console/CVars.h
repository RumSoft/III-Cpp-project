#pragma once

class CCVars final
{
public:
	CCVars() = default;
	CCVars(CCVars const&) = delete;
	CCVars(CCVars&&) = delete;
	CCVars& operator=(CCVars const&) = delete;
	CCVars& operator=(CCVars&&) = delete;

	void RegisterVariables();
	void UnregisterVariables();

	int m_debug_ai;
	int m_debug_actor;
};

extern CCVars g_cvars;