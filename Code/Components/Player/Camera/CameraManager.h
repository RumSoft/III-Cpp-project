#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
#include "GamePlugin.h"
#include "ICameraMode.h"


class CCameraManager : public IEntityComponent
{
public:
	enum ECameraMode
	{
		eCameraMode_NoCamera = -1,
		eCameraMode_Topdown,
		eCameraMode_Inventory,
		eCameraMode_Last
	};

#pragma region base
public:
	CCameraManager();
	virtual ~CCameraManager() = default;

	friend CGamePlugin;
	void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CCameraManager>& desc)
	{
		desc.SetGUID(CCameraManagerGUID);
		desc.SetEditorCategory("Actor");
		desc.SetLabel("--Camera Manager");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform, EFlags::Singleton});
	}
#pragma endregion
private:

public:
	void Update(float fFrameTime);
	void SetCameraMode(ECameraMode mode);
	bool IsActionsEnabled() { return GetCameraMode()->IsActionsEnabled();	}
	bool IsZoomEnabled()	{ return GetCameraMode()->IsZoomEnabled();		}
	bool IsCursorEnabled()	{ return GetCameraMode()->IsCursorEnabled();	}
private:
	ECameraMode GetCameraModeType() const { return m_currentMode; }
	ICameraMode* GetCameraMode() const { return m_cameraModes[m_currentMode]; }

	CCameraComponent* m_pCameraComponent = nullptr;
	CInputComponent* m_pInputComponent = nullptr;

	ICameraMode* m_cameraModes[eCameraMode_Last];
	ECameraMode m_currentMode = eCameraMode_NoCamera;

	float m_zoomDelta{ 0.0f };
};
