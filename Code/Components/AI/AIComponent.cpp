#include "StdAfx.h"
#include "AIComponent.h"
#include "Components/Actor/Actor.h"

CAIComponent::~CAIComponent()
{
}

void CAIComponent::Initialize()
{
	m_pActor = GetEntity()->GetOrCreateComponent<CActor>();
}

uint64 CAIComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_RESET);
}

void CAIComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_INIT:
	case ENTITY_EVENT_START_GAME:
	case ENTITY_EVENT_RESET:
		break;
	case ENTITY_EVENT_UPDATE:
		const auto pCtx = (SEntityUpdateContext*)event.nParam[0];
		Update(pCtx->fFrameTime);
		break;
	}
}

void CAIComponent::Update(const float fFrameTime)
{
	slowupdate += fFrameTime;
	if (slowupdate >= 5)
	{
		slowupdate = 0;
		//GetActor()->QueueAction(new MoveByAction(Vec3(4, -4, 0)));
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CAIComponent>)
