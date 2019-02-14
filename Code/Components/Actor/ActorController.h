#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/AI/PathfindingComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <CryAISystem/Components/IEntityBehaviorTreeComponent.h>
#include <CryAISystem/Components/IEntityFactionComponent.h>
#include <CryAISystem/Components/IEntityCoverUserComponent.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>

using namespace Cry::DefaultComponents;

class CActorController : public IEntityComponent
{
#pragma region base
public:
	CActorController() = default;
	virtual ~CActorController();

	friend CGamePlugin;
	void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CActorController>& desc)
	{
		desc.SetGUID(CActorControllerGUID);
		desc.SetEditorCategory("Actors");
		desc.SetLabel("Actor Controller");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform });
	}
#pragma endregion

public:

	void Revive();

	CAdvancedAnimationComponent*	GetAnimationComponent()		{ return m_pAnimation; }
	CCharacterControllerComponent*	GetCharacterController()	{ return m_pCharacterController; }
	CPathfindingComponent*			GetPathfindingComponent()	{ return m_pPathfindingComponent; }
	//IEntityBehaviorTreeComponent*	GetBehaviorTreeComponent()	{ return m_pBehaviorTree; }
	IEntityNavigationComponent*		GetNavigationComponent()	{ return m_pNavigation; }
	//IEntityCoverUserComponent*		GetCoverUserComponent()		{ return m_pCoverUser; }

private:
	CAdvancedAnimationComponent*	m_pAnimation			= nullptr;
	CCharacterControllerComponent*	m_pCharacterController	= nullptr;
	CPathfindingComponent*			m_pPathfindingComponent	= nullptr;
	//IEntityBehaviorTreeComponent*	m_pBehaviorTree			= nullptr;
	IEntityNavigationComponent*		m_pNavigation			= nullptr;
	//IEntityCoverUserComponent*	m_pCoverUser			= nullptr;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId; 
	FragmentID m_activeFragmentId;
	float m_movementSpeed = 5.f;

private:
	void UpdateAnimation(float fFrameTime);
	void Update(float fFrameTime);


};