#include "StdAfx.h"
#include "Actor.h"
#include <CryMath/Random.h>
#include "Components/Actor/Actions/IAction.h"

#include <FlashUI/FlashUI.h>
#include <AIDebugRenderer.h>
#include <UIDraw/UIDraw.h>
#include "Utils/DebugRenderer.h"
#include "Utils/Logger.h"

uint64 CActor::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_RESET);
}

void CActor::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_INIT:
	case ENTITY_EVENT_START_GAME:
	case ENTITY_EVENT_RESET:
		Revive();
		break;
	case ENTITY_EVENT_UPDATE:
		const auto pCtx = (SEntityUpdateContext*)event.nParam[0];
		Update(pCtx->fFrameTime);
		break;
	}
}

CActor::~CActor()
{
	delete _healthAttribute;
}

void CActor::Initialize()
{
	m_pActorController = GetEntity()->GetOrCreateComponentClass<CActorController>();
	m_pInventory = GetEntity()->GetOrCreateComponentClass<CInventory>();
	_healthAttribute = new CAttribute(100.f, 10.f, 2.f);
	_healthAttribute->SetAttributeEmptyCallback([this]() {
		Logger::Get().Log(GetEntity()->GetName(), "killed");
	});
	
}

void CActor::Revive()
{
	GetEntity()->Hide(false);
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));
	
}

void CActor::QueueAction(IActorAction* action)
{
	for (auto it = _actionQueue.begin(); it != _actionQueue.end(); /* NOTHING */)
		if ((*it)->GetType() == action->GetType())
			it = _actionQueue.erase(it);
		else
			++it;
	_actionQueue.push_back(action);
}

void CActor::DamageActor(const float value)
{
	_healthAttribute->ChangeValue(-value, true);
	if (_healthAttribute->IsZero()) GetEntity()->Hide(true);
}


void CActor::UpdateActions(const float fFrameTime)
{
	if (_actionQueue.empty())
		return;

	for (auto it = _actionQueue.begin(); it != _actionQueue.end(); /* NOTHING */)
	{
		if ((*it)->Process(this))
			it = _actionQueue.erase(it);
		else
			++it;
	}

}

void CActor::Update(float fFrameTime)
{
	_healthAttribute->Update(fFrameTime);
	slowupdate += fFrameTime;
	if (slowupdate >= 0.2){
		slowupdate = 0;
		UpdateActions(fFrameTime);
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActor>)
