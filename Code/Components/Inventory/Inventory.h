#pragma once

#include <CryEntitySystem/IEntitySystem.h>
#include "Components/Item/Item.h"

#define INVENTORY_CAPACITY 20

class CInventory final : public IEntityComponent
{
public:
	CInventory();
	virtual ~CInventory() = default;

	void Initialize() override;
	static void ReflectType(Schematyc::CTypeDesc<CInventory>& desc);
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	bool AddItem(SItem* pNewItem);
	bool AddItem(int slot, SItem* pNewItem);
	void RemoveItem(int slot);
	void RemoveItem(SItem* pNewItem);
	int GetItemSlot(SItem* pItem);
	SItem* GetItem(int slot);
	int GetInventorySize() { return INVENTORY_CAPACITY; };

private:
	SItem* pItems[INVENTORY_CAPACITY] = { nullptr };
};
