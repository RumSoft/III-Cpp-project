#include "StdAfx.h"
#include "Inventory.h"
#include <CryEntitySystem/IEntityBasicTypes.h>
#include <CrySchematyc/Reflection/TypeDesc.h>
#include "GamePlugin.h"

CInventory::CInventory()
{
}

void CInventory::Initialize()
{
}

void CInventory::ReflectType(Schematyc::CTypeDesc<CInventory>& desc)
{
	desc.SetGUID(CInventoryGUID);
	desc.SetEditorCategory("MyComponents");
	desc.SetLabel("CInventory");
}

uint64 CInventory::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CInventory::ProcessEvent(const SEntityEvent& event)
{
	if (event.event == ENTITY_EVENT_UPDATE)
	{
		const float frameTime = event.fParam[0];
	}
}

bool CInventory::AddItem(SItem* pNewItem)
{
	if (!pNewItem)
		return false;

	for (auto& pItem : pItems)
	{
		if (!pItem)
		{
			pItem = pNewItem;
			return true;
		}
	}

	return false;
}

bool CInventory::AddItem(const int slot, SItem* pNewItem)
{
	if (!pNewItem)
		return false;

	if (!pItems[slot])
	{
		pItems[slot] = pNewItem;
		return true;
	}
	return false;
}

void CInventory::RemoveItem(const int slot)
{
	auto item = this->GetItem(slot);
	if (slot > -1 && slot < INVENTORY_CAPACITY)
		pItems[slot] = nullptr;
	if (item)
		item->Drop();
}

void CInventory::RemoveItem(SItem* pNewItem)
{
	if (!pNewItem)
		return;

	const int slot = GetItemSlot(pNewItem);
	if (slot < 0)
		return;

	pNewItem->Drop();

	pItems[slot] = nullptr;
}

int CInventory::GetItemSlot(SItem* pItem)
{
	if (!pItem)
		return -1;
	for (auto i = 0; i < INVENTORY_CAPACITY; i++)
		if (pItems[i] == pItem)
			return i;
	return -1;
}

SItem* CInventory::GetItem(const int slot)
{
	if (slot < 0 || slot >= INVENTORY_CAPACITY)
		return nullptr;
	return pItems[slot];
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CInventory>)
