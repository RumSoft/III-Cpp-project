#pragma once
#include "Components/Item/Item.h"

class CActor;

enum ACTION_TYPE
{
	NONE = 0,
	ACTION_MOVE = 1,
	ACTION_LAST
};

class IActorAction
{
public:
	virtual ~IActorAction() = default;
	IActorAction* NextAction = nullptr;
	virtual ACTION_TYPE GetType() = 0;
	virtual bool Process(CActor* actor) = 0;
};

class ActorAction : public IActorAction
{
public:
	ActorAction() = default;
	bool Process(CActor* actor) override { return true; };
};

class MoveToAction : public ActorAction
{
	Vec3 _pos = ZERO;
public:
	MoveToAction(const Vec3 pos) : _pos(pos) {}
	bool Process(CActor* actor) override;

	ACTION_TYPE GetType() override { return ACTION_MOVE; }

};

class MoveByAction : public ActorAction
{		
	Vec3 _offset = ZERO;
public:
	MoveByAction(const Vec3 offset) : _offset(offset) {}
	bool Process(CActor* actor) override;

	ACTION_TYPE GetType() override { return ACTION_MOVE; }
};

class ChaseEntityAction : public ActorAction
{
	IEntity* _entity = nullptr;
public:
	ChaseEntityAction(IEntity* entity) : _entity(entity) {}
	bool Process(CActor* actor) override;

	ACTION_TYPE GetType() override { return ACTION_MOVE; }
};

class PickupItemAction : public ActorAction
{
	SItem* _item = nullptr;
public:
	PickupItemAction(SItem* item) : _item(item)
	{
		NextAction = new ChaseEntityAction(item->GetEntity());
	}

	bool Process(CActor* actor) override;

	ACTION_TYPE GetType() override { return ACTION_MOVE; }
};

class AttackEnemyAction : public ActorAction
{
	CActor* _enemy = nullptr;
public:
	AttackEnemyAction(CActor* enemy);

	bool Process(CActor* actor) override;

	ACTION_TYPE GetType() override { return ACTION_MOVE; }
};