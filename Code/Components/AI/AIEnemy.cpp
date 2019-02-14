#include "StdAfx.h"
#include "AIEnemy.h"


void CAIEnemy::Initialize()
{
	CAIComponent::Initialize();

}

uint64 CAIEnemy::GetEventMask() const
{
	return CAIComponent::GetEventMask();
}

void CAIEnemy::ProcessEvent(const SEntityEvent& event)
{
	CAIComponent::ProcessEvent(event);
}


CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CAIEnemy>)
