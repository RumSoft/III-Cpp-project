#pragma once
#include "ICameraMode.h"

class CTopdownCameraMode : public ICameraMode
{
public:
	CTopdownCameraMode()
	{
		_isZoomEnabled = true;
		_isActionsEnabled = true;
		_isCursorEnabled = true;
	}
	~CTopdownCameraMode() override {}
	void Update(Matrix34 playerTM) override
	{
		auto position = playerTM.GetTranslation();
		Quat rotation = Quat::CreateRotationXYZ(Ang3(_rotationX, 0, _rotationZ));

		Vec3 offset = (Vec3(cos(_rotationX), 0, -sin(_rotationX))*m_zoomDistance)
			.GetRotated(Vec3(0,0,-1), _rotationZ);
		Logger::Get().Log("Camera offset", Vec3ToString(offset));

		m_cameraMatrix = Matrix34(Vec3(1,1,1), rotation, position + offset );
	}
	void OnActivate() override
	{
		gEnv->pHardwareMouse->UseSystemCursor(_isCursorEnabled);
	}
	void OnDeactivate() override
	{
		
	}

private:
	float _rotationX = DEG2RAD(-60); //down
	float _rotationZ = DEG2RAD(45); //left-right
};