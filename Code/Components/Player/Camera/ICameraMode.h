#pragma once
#include <CryMath/Cry_Math.h>
#include <CryRenderer/Tarray.h>
#include <CrySystem/VR/IHMDDevice.h>
#include <CrySystem/VR/IHMDManager.h>
#include "DefaultComponents/Audio/ListenerComponent.h"

class ICameraMode
{
public:
	virtual ~ICameraMode() = default;
	virtual void Update(Matrix34 playerTM) = 0;
	virtual void UpdateZoom(float change);
	virtual void UpdateView(Matrix34 playerTM);
	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;

	CCamera& GetCamera() { return m_camera; }

	virtual bool IsActionsEnabled() { return _isActionsEnabled; }
	virtual bool IsZoomEnabled()	{ return _isZoomEnabled; }
	virtual bool IsCursorEnabled()	{ return _isCursorEnabled; }

protected:
	bool _isActionsEnabled	= false;
	bool _isZoomEnabled		= true;
	bool _isCursorEnabled	= false;

	float m_zoomDistance = 10;
	float m_fov = 65;
	Matrix34 m_cameraMatrix{ ZERO, IDENTITY };
	CCamera m_camera;
};

inline void ICameraMode::UpdateZoom(const float change)
{
	m_zoomDistance += change;
}

inline void ICameraMode::UpdateView(Matrix34 playerTM)
{
	Update(playerTM);

	const CCamera& systemCamera = gEnv->pSystem->GetViewCamera();

	m_camera.SetFrustum(systemCamera.GetViewSurfaceX(), systemCamera.GetViewSurfaceZ(), DEG2RAD(m_fov), 0.25,
		gEnv->p3DEngine->GetMaxViewDistance(), systemCamera.GetPixelAspectRatio());
	m_camera.SetMatrix(m_cameraMatrix);

	// Take control of the system camera.
	// TODO: This is a greedy inconsiderate way to do it. Only do this on activation and release on deactivation for the cameras.
	gEnv->pSystem->SetViewCamera(m_camera);
}
