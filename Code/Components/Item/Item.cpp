#include "StdAfx.h"
#include "Item.h"
#include <CryPhysics/physinterface.h>
#include <CryEntitySystem/IEntity.h>

void SItem::Initialize()
{
	LoadGeometry();
	SetMaterial();
	Physicalize();
}

uint64 SItem::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED);
}

void SItem::ProcessEvent(const SEntityEvent& event)
{
	if (event.event == ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED)
	{
		if (sItemProperties != sPrevItemProperties)
		{
			sPrevItemProperties = sItemProperties;
			LoadGeometry();
			SetMaterial();
			Physicalize();
		}
	}
}

void SItem::ReflectType(Schematyc::CTypeDesc<SItem>& desc)
{
	desc.SetGUID(SItemGuid);
	desc.SetEditorCategory("Items");
	desc.SetLabel("ItemComponent");
	desc.SetDescription("Base item component");
	desc.AddMember(&SItem::sItemProperties, 'itep', "ItemProperties", "Item settings", "All properties of this item.", SItemProperties());
}

bool SItem::IsPickable()
{
	return !m_pOwnerEntity;
}

void SItem::PickUp(IEntity* pNewOwner)
{
	if (!pNewOwner)
		return;
	m_pOwnerEntity = pNewOwner;
	m_pOwnerEntity->AttachChild(GetEntity());
}

void SItem::Drop()
{
	this->GetEntity()->DetachThis();
	Matrix34 tm = m_pOwnerEntity->GetWorldTM();
	this->GetEntity()->SetWorldTM(tm.AddTranslation(Vec3(1, 0, 1)));
	this->Physicalize();
	m_pOwnerEntity = nullptr;
}

void SItem::LoadGeometry()
{
	string sGeomPath = GetProperties()->sRenderProperties.sGeomPath.value.c_str();
	if (sGeomPath.empty())
	{
		m_pEntity->LoadGeometry(GEOMETRY_SLOT, "");
		return;
	}
	m_pEntity->LoadGeometry(GEOMETRY_SLOT, sGeomPath);
}

void SItem::Physicalize()
{

	SEntityPhysicalizeParams physParams;
	physParams.mass = GetProperties()->sPhysicsProperties.fMass;
	physParams.type = PE_RIGID;
	GetEntity()->Physicalize(physParams);
}

void SItem::SetMaterial()
{
	const string sMaterialPath = GetProperties()->sRenderProperties.sMaterial.value.c_str();
	const auto material = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial(sMaterialPath);
	m_pEntity->SetMaterial(material);
}


CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<SItem>)
