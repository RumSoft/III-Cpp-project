cmake_minimum_required (VERSION 3.6.2)
set(CRYENGINE_DIR "C:/Program Files (x86)/CRYENGINE Launcher/Crytek/CRYENGINE_5.5")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../")

include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Root"
		"GamePlugin.cpp"
		"StdAfx.cpp"
		"GamePlugin.h"
		"StdAfx.h"
		"Codemap.dgml"
)
add_sources("Components_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components"
)

add_sources("Actor_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Actor"
		"Components/Actor/Actor.cpp"
		"Components/Actor/ActorController.cpp"
		"Components/Actor/IActor.cpp"
		"Components/Actor/Actor.h"
		"Components/Actor/ActorController.h"
		"Components/Actor/IActor.h"
	SOURCE_GROUP "Components\\\\Actor\\\\Actions"
		"Components/Actor/Actions/IAction.cpp"
		"Components/Actor/Actions/IAction.h"
	SOURCE_GROUP "Components\\\\Actor\\\\State"
		"Components/Actor/State/IActorState.h"
)

add_sources("AI_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\AI"
		"Components/AI/AIComponent.cpp"
		"Components/AI/AIEnemy.cpp"
		"Components/AI/AIComponent.h"
		"Components/AI/AIEnemy.h"
		"Components/AI/BehaviorTree.h"
    SOURCE_GROUP "Components\\\\AI\\\\Nodes"
		"Components/AI/Nodes/TestNode.cpp"
		"Components/AI/Nodes/TestNode.h"
	SOURCE_GROUP "Components\\\\AI\\\\Trees"
		"Components/AI/Trees/ITree.h"
)

add_sources("Items_uber.cpp"
	PROJECTS Game
	SOURCE_GROUP "Components\\\\Item"
		"Components/Item/Item.cpp"
		"Components/Item/Item.h"
		"Components/Item/ItemProperties.h"
)

add_sources("Additional_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Attributes"
		"Components/Attributes/Attributes.h"
	SOURCE_GROUP "Components\\\\Inventory"
		"Components/Inventory/Inventory.cpp"
		"Components/Inventory/Inventory.h"
)

add_sources("Player_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Player"
		"Components/Player/Player.cpp"
		"Components/Player/Player.h"
	SOURCE_GROUP "Components\\\\Player\\\\Camera"
		"Components/Player/Camera/CameraManager.cpp"
		"Components/Player/Camera/CameraManager.h"
		"Components/Player/Camera/ICameraMode.h"
		"Components/Player/Camera/TopdownCameraMode.h"
		"Components/Player/Camera/InventoryCameraMode.h"
	SOURCE_GROUP "Components\\\\Player\\\\Input"
		"Components/Player/Input/Input.h"
)
add_sources("Console_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Console"
		"Console/CVars.cpp"
		"Console/CVars.h"
)
add_sources("Utils_uber.cpp" 
    PROJECTS Game
    SOURCE_GROUP "Utils"
		"Utils/Logger.h"
		"Utils/MovingAverage.h"
		"Utils/StringConversions.h"
		"Utils/DebugRenderer.h"
)

end_sources()

CryEngineModule(Game FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
		add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
		set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
		if (WIN32)
			set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}Game.cryproject\"")
		endif()
	endif()
else()
	add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
	set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
	if (WIN32)
		set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}Game.cryproject\"")
	endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}Game.cryproject\"")
    endif()

	add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
	set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
	if (WIN32)
		set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"${PROJECT_DIR}Game.cryproject\"")
	endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}Game.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
#END-CUSTOM