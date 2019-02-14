#pragma once

#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/Reflection/TypeDesc.h>
#include "GamePlugin.h"

enum ItemGeometrySlot
{
	GEOMETRY_SLOT = 0
};


//RENDER
struct SRenderProperties
{
	inline bool operator==(const SRenderProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SRenderProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }
	Schematyc::GeomFileName sGeomPath;
	Schematyc::MaterialFileName sMaterial;
};
static void ReflectType(Schematyc::CTypeDesc<SRenderProperties>& desc)
{
	desc.SetGUID(SItemSRenderPropsGuid);
	desc.SetLabel("Render properties");
	desc.SetDescription("Render properties");
	desc.AddMember(&SRenderProperties::sGeomPath, 'geom', "GeometryPath", "Geometry path", "Geometry path of this item.", "");
	desc.AddMember(&SRenderProperties::sMaterial, 'smat', "MaterialPath", "Material path", "Material path of this item.", "");
}

// PHYSICS
struct SPhysicsProperties
{
	inline bool operator==(const SPhysicsProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SPhysicsProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	float fMass;
};
static void ReflectType(Schematyc::CTypeDesc<SPhysicsProperties>& desc)
{
	desc.SetGUID(SItemSPhysicsPropsGuid);
	desc.SetLabel("Render properties");
	desc.SetDescription("Render properties");
	desc.AddMember(&SPhysicsProperties::fMass, 'mass', "Mass", "Mass", "Mass property of this item.", 1.f);
}


//MAIN PROPERTY STRUCTURE
struct SItemProperties
{
	SItemProperties()
	{
		ItemId = -1;
		prevItemId = -1;
		sRenderProperties = SRenderProperties();
		sPhysicsProperties = SPhysicsProperties();
	}
	inline bool operator==(const SItemProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SItemProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	int ItemId, prevItemId;
	SRenderProperties sRenderProperties;
	SPhysicsProperties sPhysicsProperties;
};
static void ReflectType(Schematyc::CTypeDesc<SItemProperties>& desc)
{
	desc.SetGUID(SItemPropertiesGuid);
	desc.SetLabel("Item properties");
	desc.SetDescription("Item properties");
	desc.AddMember(&SItemProperties::sPhysicsProperties, 'phyp', "PhysicsProperties", "Physics settings", "All physics settings of this item.", SPhysicsProperties());
	desc.AddMember(&SItemProperties::sRenderProperties, 'renp', "RenderProperties", "Render settings", "All render settings of this item.", SRenderProperties());
	desc.AddMember(&SItemProperties::ItemId, 'itid', "ItemId", "Item id", "Item ID that defines mass, geometry etc.", 0);
}