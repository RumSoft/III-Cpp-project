#pragma once
#include "GamePlugin.h"

#define debug

struct IActor : IEntityComponent
{
#pragma region base
public:
	IActor() = default;
	virtual ~IActor() = default;

protected:
	friend CGamePlugin;
	static void ReflectType(Schematyc::CTypeDesc<IActor>& desc)
	{
		desc.SetGUID(IActorGUID);
		desc.SetEditorCategory("Actors");
		desc.SetLabel("Actor");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform });
	}
#pragma endregion

public:

	virtual void Revive() = 0;
protected:
	virtual void Update(float fFrameTime) = 0;

};
