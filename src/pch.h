#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS 1
#endif

#ifndef HID_USAGE_PAGE_GENERIC
# define HID_USAGE_PAGE_GENERIC  ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
# define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif

#include <string>
#include <array>
#include <algorithm>
#include <functional>


#ifndef NOMINMAX
# define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <windowsx.h>
#include <winuser.h>

#include <direct.h>
#include <d3d9.h>
#include <imgui.h>
#include <imgui_internal.h>
#include "imgui/imgui_impl_Win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.cpp"

#include "hooks.h"
#include "draw.h"

// GWCA include fix
#pragma warning( push, 0 )

#include "GWCA/Constants/Constants.h"

#include "GWCA/Utilities/Debug.h"
#include "GWCA/Utilities/Export.h"
#include "GWCA/Utilities/Hook.h"
#include "GWCA/Utilities/Hooker.h"
#include "GWCA/Utilities/MemoryPatcher.h"
#include "GWCA/Utilities/Scanner.h"
#include "GWCA/Utilities/Macros.h"

#include "GWCA/GameContainers/Array.h"
#include "GWCA/GameContainers/GamePos.h"
#include "GWCA/GameContainers/List.h"

#include "GWCA/GameEntities/Agent.h"
#include "GWCA/GameEntities/Attribute.h"
#include "GWCA/GameEntities/Camera.h"
#include "GWCA/GameEntities/Friendslist.h"
#include "GWCA/GameEntities/Guild.h"
#include "GWCA/GameEntities/Hero.h"
#include "GWCA/GameEntities/Item.h"
#include "GWCA/GameEntities/Map.h"
#include "GWCA/GameEntities/Match.h"
#include "GWCA/GameEntities/NPC.h"
#include "GWCA/GameEntities/Party.h"
#include "GWCA/GameEntities/Pathing.h"
#include "GWCA/GameEntities/Player.h"
#include "GWCA/GameEntities/Quest.h"
#include "GWCA/GameEntities/Skill.h"
#include "GWCA/GameEntities/Title.h"

#include "GWCA/Context/AccountContext.h"
#include "GWCA/Context/AgentContext.h"
#include "GWCA/Context/CharContext.h"
#include "GWCA/Context/Cinematic.h"
#include "GWCA/Context/GadgetContext.h"
#include "GWCA/Context/GameContext.h"
#include "GWCA/Context/GuildContext.h"
#include "GWCA/Context/ItemContext.h"
#include "GWCA/Context/MapContext.h"
#include "GWCA/Context/PartyContext.h"
#include "GWCA/Context/TextParser.h"
#include "GWCA/Context/TradeContext.h"
#include "GWCA/Context/WorldContext.h"

#include "GWCA/Packets/Opcodes.h"
#include "GWCA/Packets/StoC.h"

#include "GWCA/Managers/Module.h"
#include "GWCA/Managers/AgentMgr.h"
#include "GWCA/Managers/CameraMgr.h"
#include "GWCA/Managers/ChatMgr.h"
#include "GWCA/Managers/CtoSMgr.h"
#include "GWCA/Managers/EffectMgr.h"
#include "GWCA/Managers/EventMgr.h"
#include "GWCA/Managers/FriendListMgr.h"
#include "GWCA/Managers/GameThreadMgr.h"
#include "GWCA/Managers/GuildMgr.h"
#include "GWCA/Managers/ItemMgr.h"
#include "GWCA/Managers/MapMgr.h"
#include "GWCA/Managers/MemoryMgr.h"
#include "GWCA/Managers/MerchantMgr.h"
#include "GWCA/Managers/PartyMgr.h"
#include "GWCA/Managers/PlayerMgr.h"
#include "GWCA/Managers/QuestMgr.h"
#include "GWCA/Managers/RenderMgr.h"
#include "GWCA/Managers/SkillbarMgr.h"
#include "GWCA/Managers/StoCMgr.h"
#include "GWCA/Managers/TradeMgr.h"
#include "GWCA/Managers/UIMgr.h"

#include "GWCA/GWCA.h"
#pragma warning(pop)



#pragma warning(disable: 4061) // enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable: 4464) // relative include path contains '..'
#pragma warning(disable: 4514) // 'function' : unreferenced inline function has been removed
#pragma warning(disable: 4505) // 'function' : unreferenced local function has been removed
#pragma warning(disable: 4530) // C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
#pragma warning(disable: 4577) // 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed. Specify /EHsc
#pragma warning(disable: 4623) // 'derived class' : default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
#pragma warning(disable: 4626) // 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
#pragma warning(disable: 4710) // 'function' : function not inlined
#pragma warning(disable: 4711) // function 'function' selected for inline expansion
#pragma warning(disable: 4774) // 'string' : format string expected in argument number is not a string literal
#pragma warning(disable: 4820) // 'bytes' bytes padding added after construct 'member_name'
#pragma warning(disable: 5027) // 'type': move assignment operator was implicitly defined as deleted
#pragma warning(disable: 5029) // nonstandard extension used: alignment attributes in C++ apply to variables, data members and tag types only
#pragma warning(disable: 5045) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning(disable: 6318) // Ill-defined __try/__except. The code in the exception handler block is not executed