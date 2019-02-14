#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include "GamePlugin.h"
#include "Components/Actor/Actor.h"


class CAIComponent : public IEntityComponent
{
#pragma region base
public:
	CAIComponent() = default;
	virtual ~CAIComponent();

	friend CGamePlugin;
	void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CAIComponent>& desc)
	{
		desc.SetGUID(CAIComponentGUID);
		desc.SetEditorCategory("Actors");
		desc.SetLabel("AI Actor");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform });
	}
#pragma endregion

	CActor* GetActor() { return m_pActor; }
	void Update(float fFrameTime);

private:
	CActor* m_pActor = nullptr;
	float slowupdate = 0;
};
