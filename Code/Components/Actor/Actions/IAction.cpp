#include "StdAfx.h"
#include "IAction.h"
#include "Components/Actor/Actor.h"

//1 - navigation component
//2 - pathfinding component
#define navType 2

#if navType == 2
#define nav GetNavigationComponent()
#define moveTo NavigateTo
#else
#define nav GetPathfindingComponent()
#define moveTo RequestMoveTo
#endif

#define controller GetController()

//if less than 1m, do not move
#define MOVE_TRESHOLD 1
#define LEN GetLength2D()
#define DISTANCE(a,b) (a - b).LEN
#define POS GetWorldPos()
#define ENT GetEntity()
#define INV GetInventory()

bool MoveToAction::Process(CActor* actor)
{
	if (DISTANCE(actor->ENT->POS, _pos) < MOVE_TRESHOLD)
		return true;
	actor->controller->nav->moveTo(_pos);
	return false;
}

bool MoveByAction::Process(CActor* actor)
{
	if (_offset.LEN < MOVE_TRESHOLD)
		return true;
	actor->controller->nav->moveTo(actor->ENT->POS + _offset);
	return false;
}

bool ChaseEntityAction::Process(CActor* actor)
{
	if (DISTANCE(actor->ENT->POS, _entity->POS) < MOVE_TRESHOLD)
		return true;
	actor->controller->nav->moveTo(_entity->POS);
	return false;
}

bool PickupItemAction::Process(CActor* actor)
{
	if (!_item || !_item->IsPickable())
		return true; //return true to finish action

	if (NextAction->Process(actor))
	{
		if (actor->INV->AddItem(_item)) {
			_item->PickUp(actor->ENT);
		}
		else
		{
			//couldn't pick up, return true to finish
		}
		return true;
	}
	return false;
}

AttackEnemyAction::AttackEnemyAction(CActor* enemy): _enemy(enemy)
{
	NextAction = new ChaseEntityAction(enemy->GetEntity());
}

bool AttackEnemyAction::Process(CActor* actor)
{
	if (!_enemy)
		return true;
	if(NextAction->Process(actor))
	{
		if (_enemy->GetHealth()->IsZero())
			return true;

		_enemy->DamageActor(10);
		pe_action_impulse impulse;
		impulse.impulse = Vec3(0, 0, 10);
		_enemy->GetEntity()->GetPhysics()->Action(&impulse);
	}
	return false;
}


