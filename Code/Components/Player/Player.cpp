#include "StdAfx.h"
#include "Player.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CryEntitySystem/IEntity.h>
#include <CryAction/IActionMapManager.h>
#include "Utils/Logger.h"
#include "Utils/StringConversions.h"
#include <CryInput/IHardwareMouse.h>
#include "Components/Item/Item.h"
#include "Components/AI/AIComponent.h"
#include "Components/AI/AIEnemy.h"
#include <CryString/CryString.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include "Components/Actor/Actions/IAction.h"

CPlayerComponent::~CPlayerComponent()
{

}

void CPlayerComponent::Initialize()
{
	m_pCameraManager = GetEntity()->GetOrCreateComponent<CCameraManager>();
	m_pInputComponent = m_pEntity->GetOrCreateComponent<CInputComponent>();
	m_pCursorComponent = m_pEntity->GetOrCreateComponent<CProjectorLightComponent>();
	m_pActor = GetEntity()->GetOrCreateComponent<CActor>();

	m_pInputComponent->RegisterAction("player", "walkto", [this](const int activationMode, float value)
	{
		if (activationMode == eAAM_OnPress)
		{
			_click = true;
			if (!_mouseHitTarget)
				QueueAction(new MoveToAction(_mousePos));
		}
		else
			_click = false;
	});
	m_pInputComponent->BindAction("player", "walkto", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);

	m_pInputComponent->RegisterAction("camera", "inventory", [this](int activationmode, float) {
		if (activationmode == eIS_Released)
		{
			inventoryMode = !inventoryMode;
			GetCameraManager()->SetCameraMode(inventoryMode
				? CCameraManager::eCameraMode_Inventory
				: CCameraManager::eCameraMode_Topdown);
		}
	});
	m_pInputComponent->BindAction("camera", "inventory", eAID_KeyboardMouse, EKeyId::eKI_I);


	m_pInputComponent->RegisterAction("inventory", "inventory_moveup", [this](int activationmode, float) {
		if (activationmode == eIS_Released && inventoryMode)
			_invSelId = (_invSelId - 1 + GetActor()->GetInventory()->GetInventorySize()) % GetActor()->GetInventory()->GetInventorySize();
	});
	m_pInputComponent->RegisterAction("inventory", "inventory_movedown", [this](int activationmode, float) {
		if (activationmode == eIS_Released && inventoryMode)
			_invSelId = (_invSelId + 1) % GetActor()->GetInventory()->GetInventorySize();
	});
	m_pInputComponent->RegisterAction("inventory", "inventory_drop", [this](int activationmode, float) {
		if (activationmode == eIS_Released && inventoryMode)
		{
			GetActor()->GetInventory()->RemoveItem(GetActor()->GetInventory()->GetItem(_invSelId));
		}
	});
	m_pInputComponent->RegisterAction("inventory", "inventory_equip", [this](int activationmode, float) {
		if (activationmode == eIS_Released && inventoryMode)
		{
			//GetActor()->GetInventory()->
		}
	});
	m_pInputComponent->BindAction("inventory", "inventory_moveup", eAID_KeyboardMouse, EKeyId::eKI_Up);
	m_pInputComponent->BindAction("inventory", "inventory_movedown", eAID_KeyboardMouse, EKeyId::eKI_Down);
	m_pInputComponent->BindAction("inventory", "inventory_equip", eAID_KeyboardMouse, EKeyId::eKI_Enter);
	m_pInputComponent->BindAction("inventory", "inventory_drop", eAID_KeyboardMouse, EKeyId::eKI_X);



	m_pInputComponent->RegisterAction("skill", "skill1", [this](int activationmode, float) { if (activationmode == eIS_Released && !inventoryMode) { UseSkill(1); } });
	m_pInputComponent->RegisterAction("skill", "skill2", [this](int activationmode, float) { if (activationmode == eIS_Released && !inventoryMode) { UseSkill(2); } });
	m_pInputComponent->RegisterAction("skill", "skill3", [this](int activationmode, float) { if (activationmode == eIS_Released && !inventoryMode) { UseSkill(3); } });
	m_pInputComponent->RegisterAction("skill", "skill4", [this](int activationmode, float) { if (activationmode == eIS_Released && !inventoryMode) { UseSkill(4); } });
	m_pInputComponent->BindAction("skill", "skill1", eAID_KeyboardMouse, EKeyId::eKI_1);
	m_pInputComponent->BindAction("skill", "skill2", eAID_KeyboardMouse, EKeyId::eKI_2);
	m_pInputComponent->BindAction("skill", "skill3", eAID_KeyboardMouse, EKeyId::eKI_3);
	m_pInputComponent->BindAction("skill", "skill4", eAID_KeyboardMouse, EKeyId::eKI_4);

	_invBgTex = gEnv->pRenderer->EF_LoadTexture("engineassets/textures/grey.dds");

	Revive();

}

uint64 CPlayerComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_RESET);
}

void CPlayerComponent::Update(const float fFrameTime)
{
	gEnv->pAuxGeomRenderer->Draw2dLabel(10, 10, 1.75, ColorF(1, 1, 1), false, Logger::Get().ReadLog());
	gEnv->pAuxGeomRenderer->Draw2dLabel(30, 300, 1.75, ColorF(1, 1, 1), false, Snackbar::Get().ReadLog());
	if(GetCameraManager()->IsCursorEnabled())
	UpdateMouse(fFrameTime);
}

void CPlayerComponent::QueueAction(IActorAction* action)
{

	if (GetCameraManager()->IsActionsEnabled())
		GetActor()->QueueAction(action);
}

void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_RESET:
		// Check if we're entering game mode
		break;
	case ENTITY_EVENT_START_GAME:
		Revive();
		break;
	case ENTITY_EVENT_UPDATE:
		const auto pCtx = reinterpret_cast<SEntityUpdateContext*>(event.nParam[0]);
		Update(pCtx->fFrameTime);
		DrawHealth();
		if (inventoryMode)
			DrawInventory();
		break;
	}
}

void CPlayerComponent::DrawInventory()
{
	const int margin = 20;	
	const Vec2 bounds[] = { Vec2(margin,margin), Vec2(gEnv->pRenderer->GetWidth() / 2 - 2 * margin, gEnv->pRenderer->GetHeight() - 2 * margin) };

	//bg
	IRenderAuxImage::Draw2dImage(bounds[0].x, bounds[0].y, bounds[1].x, bounds[1].y, _invBgTex->GetTextureID(), 0, 0, 1, 1, 0, .5, .5, .5, 0.8, 1, 0);
	
	//title
	IRenderAuxText::Draw2dLabel(bounds[0].x + margin, bounds[0].y + margin, 4, ColorF(1, 1, 1), false, "Player inventory");

	//columns names
	IRenderAuxText::Draw2dLabel(bounds[0].x + margin, bounds[0].y + margin + 70, 2, ColorF(1, 1, 1), false, "Sel Eqp Item name");

	//items
	CryStringT<char> str;
	const auto inv = GetActor()->GetInventory();
	for(int i = 0; i<inv->GetInventorySize(); i++)
		str.AppendFormat("%s  %s  %s \n", 
			i == _invSelId ? "[#]" : "[ ]", 
			inv->GetItem(i) ? "[ ]" : "  " , 
			inv->GetItem(i) ? inv->GetItem(i)->GetEntity()->GetName() : "");\
	IRenderAuxText::Draw2dLabel(bounds[0].x + margin, bounds[0].y + margin + 100, 2, ColorF(1, 1, 1), false, str);

	//options
	IRenderAuxText::Draw2dLabel(bounds[0].x + margin, bounds[0].y + margin + 600, 2, ColorF(1, 1, 1), false, "X - drop item\nENTER - equip item");

}

void CPlayerComponent::DrawHealth()
{
	auto health = GetActor()->GetHealth();
	auto bottomMid = Vec2(gEnv->pRenderer->GetWidth() / 2, gEnv->pRenderer->GetHeight() - 100);
	CryStringT<char> healthStr = "[";
	for(int i = 0; i<20; i++)
		if ((20 * health->GetValue()) / health->GetMaxValue() >= i)
			healthStr.append("#");
		else
			healthStr.append("_");
	healthStr.append("]");
	gEnv->pAuxGeomRenderer->Draw2dLabel(bottomMid.x, bottomMid.y, 2, ColorF(1, 0.2, 0.2), true, healthStr);
}


void CPlayerComponent::UseSkill(int n)
{

}


void CPlayerComponent::Revive()
{

	// Set player transformation, but skip this in the Editor
	if (!gEnv->IsEditor())
	{
		const Vec3 playerScale = Vec3(1.f);
		const Quat playerRotation = IDENTITY;

		// Offset the player in the center of the map
		const float heightOffset = 20.f;
		const float terrainCenter = gEnv->p3DEngine->GetTerrainSize() / 2.f;
		const float height = gEnv->p3DEngine->GetTerrainZ(terrainCenter, terrainCenter);
		const Vec3 playerPosition = Vec3(terrainCenter, terrainCenter, height + heightOffset);

		m_pEntity->SetWorldTM(Matrix34::Create(playerScale, playerRotation, playerPosition));


	}
	GetEntity()->Hide(false);
}

void CPlayerComponent::UpdateMouse(float fFrameTime)
{

	gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&_mouseScreen.x, &_mouseScreen.y);

	// Invert mouse Y
	_mouseScreen.y = gEnv->pRenderer->GetHeight() - _mouseScreen.y;

	Vec3 p1(0, 0, 0), p2(0, 0, 0);
	gEnv->pRenderer->UnProjectFromScreen(_mouseScreen.x, _mouseScreen.y, 0, &p1.x, &p1.y, &p1.z);
	gEnv->pRenderer->UnProjectFromScreen(_mouseScreen.x, _mouseScreen.y, 1, &p2.x, &p2.y, &p2.z);

	Vec3 dir = p2 - p1;
	dir.Normalize();

	if (gEnv->pPhysicalWorld->RayWorldIntersection(p1, dir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all,
		rwi_stop_at_pierceable | rwi_colltype_any, &_mouseRaycastHit, 1))
	{
		_mousePos = _mouseRaycastHit.pt;

		gEnv->pAuxGeomRenderer->DrawSphere(_mousePos, 0.1, ColorB(255, 0, 255), false);
		UpdateCursor();

		if (_mouseRaycastHit.pCollider)
		{
			if ((_mouseHitTarget = gEnv->pEntitySystem->GetEntityFromPhysics(_mouseRaycastHit.pCollider))) {

				if (const auto item = _mouseHitTarget->GetComponent<SItem>())
				{
					if (_click)
						QueueAction(new PickupItemAction(item));
					IRenderAuxText::DrawLabel(_mouseHitTarget->GetWorldPos(), 2, _mouseHitTarget->GetName());
				}
				if (const auto aiEnemy = _mouseHitTarget->GetComponent<CAIEnemy>())
				{
					if (_click)
						QueueAction(new AttackEnemyAction(aiEnemy->GetActor()));

					auto health = aiEnemy->GetActor()->GetHealth();
					CryStringT<char> healthStr = "[";
					for (int i = 0; i < 20; i++)
						if ((20 * health->GetValue()) / health->GetMaxValue() >= i)
							healthStr.append("#");
						else
							healthStr.append("_");
					healthStr.append("]");

					IRenderAuxText::DrawLabelF(aiEnemy->GetEntity()->GetWorldPos() + Vec3(0, 0, 2), 2, " %s \n %s",
						aiEnemy->GetEntity()->GetName(), healthStr);

	
				}
			}
		}
	}
}

void CPlayerComponent::HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type)
{

}


void CPlayerComponent::UpdateCursor()
{
	const auto offset = GetEntity()->GetWorldPos() - _mousePos;
	const auto dir = GetEntity()->GetWorldRotation();
	m_pCursorComponent->SetTransformMatrix(
		Matrix34(Vec3(1, 1, 1),
			Quat::CreateRotationY(DEG2RAD(90)),
			Vec3(0, 0, 1) - offset * dir));
}

