#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "GamePlugin.h"
#include "ItemProperties.h"

class SItem : public IEntityComponent
{
public:
	SItem() = default;
	~SItem() = default;

	
	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<SItem>& desc);

	virtual bool IsPickable();
	virtual void PickUp(IEntity* pNewOwner);
	virtual void Drop();

	SItemProperties* GetProperties() { return &sItemProperties; }

protected:
	void LoadGeometry();
	void Physicalize();
	void SetMaterial();

	SItemProperties sItemProperties, sPrevItemProperties;
	SPhysicsProperties sPhysicsProperties;
	IEntity* m_pOwnerEntity = nullptr;
	IMaterial* m_pMaterial;
};
