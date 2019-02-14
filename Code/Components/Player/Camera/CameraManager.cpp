#include "StdAfx.h"
#include "CameraManager.h"
#include "TopdownCameraMode.h"
#include "InventoryCameraMode.h"

CCameraManager::CCameraManager()
{
	memset(m_cameraModes, 0, sizeof(m_cameraModes));
}

void CCameraManager::Initialize()
{
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
	m_pInputComponent->RegisterAction("camera", "tpv_zoom_in", [this](int, float) { m_zoomDelta -= 1.0f; });
	m_pInputComponent->BindAction("camera", "tpv_zoom_in", eAID_KeyboardMouse, EKeyId::eKI_MouseWheelUp);
	m_pInputComponent->RegisterAction("camera", "tpv_zoom_out", [this](int, float) { m_zoomDelta += 1.0f; });
	m_pInputComponent->BindAction("camera", "tpv_zoom_out", eAID_KeyboardMouse, EKeyId::eKI_MouseWheelDown);


	m_cameraModes[eCameraMode_Topdown] = new CTopdownCameraMode();
	m_cameraModes[eCameraMode_Inventory] = new CInventoryCameraMode();
	m_currentMode = eCameraMode_Topdown;
}

uint64 CCameraManager::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CCameraManager::ProcessEvent(const SEntityEvent& event)
{
	const auto pCtx = (SEntityUpdateContext*)event.nParam[0];
	switch(event.event)
	{
	case ENTITY_EVENT_UPDATE:
		Update(pCtx->fFrameTime);
		break;
	}
}

void CCameraManager::SetCameraMode(ECameraMode mode)
{
	if (m_currentMode == mode 
		|| m_currentMode <= eCameraMode_NoCamera 
		|| m_currentMode >= eCameraMode_Last)
		return;
	
	GetCameraMode()->OnDeactivate();
	m_currentMode = mode;
	GetCameraMode()->OnActivate();
}

void CCameraManager::Update(float fFrameTime)
{
	GetCameraMode()->UpdateZoom(m_zoomDelta);
	GetCameraMode()->UpdateView(GetEntity()->GetWorldTM());
	m_zoomDelta = 0;
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CCameraManager>)
