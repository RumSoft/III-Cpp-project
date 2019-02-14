#pragma once

#include <CrySystem/ICryPlugin.h>
#include <CryGame/IGameFramework.h>
#include <CryEntitySystem/IEntityClass.h>
#include <CryNetwork/INetwork.h>

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <Utils/Logger.h>

class CPlayerComponent;

// The entry-point of the application
// An instance of CGamePlugin is automatically created when the library is loaded
// We then construct the local player entity and CPlayerComponent instance when OnClientConnectionReceived is first called.
class CGamePlugin 
	: public Cry::IEnginePlugin
	, public ISystemEventListener
	, public INetworkedClientListener
{
public:
	CRYINTERFACE_SIMPLE(Cry::IEnginePlugin)
	CRYGENERATE_SINGLETONCLASS_GUID(CGamePlugin, "Blank", "f01244b0-a4e7-4dc6-91e1-0ed18906fe7c"_cry_guid)

	virtual ~CGamePlugin();
	
	// Cry::IEnginePlugin
	virtual const char* GetCategory() const override { return "Game"; }
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	// ~Cry::IEnginePlugin

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	static Logger& Logger()	{ return Logger::Get(); }
	static Snackbar& Snackbar() { return Snackbar::Get(); }

	// INetworkedClientListener
	// Sent to the local client on disconnect
	virtual void OnLocalClientDisconnected(EDisconnectionCause cause, const char* description) override {}

	// Sent to the server when a new client has started connecting
	// Return false to disallow the connection
	virtual bool OnClientConnectionReceived(int channelId, bool bIsReset) override;
	// Sent to the server when a new client has finished connecting and is ready for gameplay
	// Return false to disallow the connection and kick the player
	virtual bool OnClientReadyForGameplay(int channelId, bool bIsReset) override;
	// Sent to the server when a client is disconnected
	virtual void OnClientDisconnected(int channelId, EDisconnectionCause cause, const char* description, bool bKeepClient) override;
	// Sent to the server when a client is timing out (no packets for X seconds)
	// Return true to allow disconnection, otherwise false to keep client.
	virtual bool OnClientTimingOut(int channelId, EDisconnectionCause cause, const char* description) override { return true; }
	// ~INetworkedClientListener
protected:
	// Map containing player components, key is the channel id received in OnClientConnectionReceived
	std::unordered_map<int, EntityId> m_players;

};

template<typename T>
static void registerComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(T));
		{

		}
	}
}

#define IActorGUID				"{57AE3A90-AB96-468F-AAA0-3BABA73B0551}"_cry_guid
#define CActorGUID				"{903918CF-FC5F-4C9D-880F-23AF970B409D}"_cry_guid
#define CActorControllerGUID	"{712DF623-25C7-41C7-A085-9E27517258BE}"_cry_guid
#define CAIComponentGUID		"{AACB7C55-4E30-4A2A-93B9-2571631DB3BA}"_cry_guid
#define CEnemyGUID				"{4F45D216-233B-439C-931E-96A0867A6846}"_cry_guid
#define CInventoryGUID			"{620546B7-7A34-4472-AFE8-65BB8223B55D}"_cry_guid
#define CAttributesGUID			"{6D1B110E-5F79-4E34-AE7B-9B4762561410}"_cry_guid
#define SItemGuid				"{33DAABF8-7BD2-4DFE-A338-BF298585B8B1}"_cry_guid
#define SItemPropertiesGuid		"{6CC9E58A-7390-463F-BD8D-064B89DF6D31}"_cry_guid
#define SItemSPhysicsPropsGuid	"{B6EC37B5-F436-4AAC-AF2C-4D7EE73F5E42}"_cry_guid
#define SItemSRenderPropsGuid	"{309F8F44-AA34-47AA-86C9-0315D0D6B66C}"_cry_guid
#define CCameraManagerGUID		"{9DB97454-C11B-445F-83D1-166B1E059075}"_cry_guid
#define CMeleeWeaponGUID		"{B3525EE9-4C30-46C5-BA37-E0163FF57B5A}"_cry_guid
#define CMeleeWeaponPropsGUID	"{43028B36-3957-4ECF-A1CC-26701859D616}"_cry_guid