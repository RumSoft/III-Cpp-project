#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryMath/Cry_Camera.h>

#include <ICryMannequin.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Lights/ProjectorLightComponent.h>
#include "Components/Actor/Actor.h"
#include "Camera/CameraManager.h"

class CGamePlugin;

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CPlayerComponent final : public IEntityComponent
{
	enum class EInputFlagType
	{
		Hold = 0,
		Toggle
	};

	typedef uint8 TInputFlags;

	enum class EInputFlag
		: TInputFlags
	{
		MoveLeft = 1 << 0,
		MoveRight = 1 << 1,
		MoveForward = 1 << 2,
		MoveBack = 1 << 3
	};



public:
	CPlayerComponent() = default;
	virtual ~CPlayerComponent();

	friend CGamePlugin;
	// IEntityComponent
	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;

	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc)
	{
		desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);
	}

	
	void Revive();
	void UpdateMouse(float fFrameTime);
	void Update(float fFrameTime);
	void QueueAction(IActorAction* action);

	CActor* GetActor() { return m_pActor; }
	CCameraManager* GetCameraManager() { return m_pCameraManager; }
protected:
	void HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type = EInputFlagType::Hold);

private:
	CCameraManager* m_pCameraManager = nullptr;
	CInputComponent* m_pInputComponent = nullptr;
	CProjectorLightComponent* m_pCursorComponent = nullptr;
	
	CActor* m_pActor = nullptr;
	 
	Vec2 _mouseScreen = ZERO;
	Vec3 _mousePos = ZERO;
	ray_hit _mouseRaycastHit{};
	IEntity* _mouseHitTarget = nullptr;

	bool inventoryMode = false;
	bool _click = false;
	void UpdateCursor();
	void DrawInventory();
	void DrawHealth();

	void UseSkill(int n);

	ITexture* _invBgTex;
	int _invSelId = 0;
};
