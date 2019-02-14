#pragma once
#include "ICameraMode.h"
#include <FlashUI/FlashUI.h>

class CInventoryCameraMode : public ICameraMode
{
public:
	CInventoryCameraMode()
	{
		_isZoomEnabled = false;
		_isActionsEnabled = false;
		_isCursorEnabled = false;
	};
	~CInventoryCameraMode() = default;

	void Update(Matrix34 playerTM) override
	{
		auto rot = Quat(playerTM);
		const auto offset = Vec3(2, 5, 0).GetRotated(Vec3(0, 0, 1), rot.GetRotZ());
		const auto cameraPos = Vec3(0, 0, 1) + playerTM.GetTranslation() + offset;
		const auto rotation = rot.CreateRotationZ(rot.GetRotZ() + DEG2RAD(180));
		m_cameraMatrix = Matrix34(Vec3(1, 1, 1), rotation, cameraPos);
	};

	void OnActivate() override
	{
		gEnv->pConsole->ExecuteString("t_scale 0.1");
	}
	void OnDeactivate() override
	{
		gEnv->pConsole->ExecuteString("t_scale 1");
	}
};