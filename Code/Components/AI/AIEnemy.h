#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "GamePlugin.h"
#include "AIComponent.h"

class CAIEnemy : public CAIComponent
{
#pragma region base
public:
	CAIEnemy() = default;
	virtual ~CAIEnemy() = default;

	friend CGamePlugin;
	void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CAIEnemy>& desc)
	{
		desc.SetGUID(CEnemyGUID);
		desc.SetEditorCategory("Actors");
		desc.SetLabel("Enemy AI Actor");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform });
	}
#pragma endregion

public:

private:
};
