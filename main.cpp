#pragma once
#include <windows.h>
#include "detours\detours.h"
#include "helper.h"
#include "GarrysMod/Lua/Interface.h"
#pragma comment(lib, "detours.lib")

class ILuaBase;

typedef int (*luaL_loadbufferx_proto)(ILuaBase*, const char*, size_t, const char*, const char*);
luaL_loadbufferx_proto luaL_loadbufferx;

DLL_EXPORT int luaL_loadbufferx_hook(ILuaBase* obj, const char* body, size_t size, const char* name, const char* mode) {
	bool orig = false;

	if (*name == '@') {
		++name;
		orig = true;
	}

	std::string comment = std::string(name);
	path file_path = "lua_dumping";
	file_path /= comment;

	safe_file_write(file_path, body, comment);

	if(orig)
		--name;

	return luaL_loadbufferx(obj, body, size, name, mode);
}

/*BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD hReason, LPVOID lpFree) {
	if (hReason == DLL_PROCESS_ATTACH) {
		//CreateThread(NULL, NULL, Thread, NULL, NULL, NULL);
		luaL_loadbufferx = (luaL_loadbufferx_proto)GetProcAddress(GetModuleHandleA("lua_shared.dll"), "luaL_loadbufferx");
		luaL_loadbufferx = (luaL_loadbufferx_proto)DetourFunction((PBYTE)luaL_loadbufferx, (PBYTE)&luaL_loadbufferx_hook);
	}

	return TRUE;
}*/

GMOD_MODULE_OPEN() {
	luaL_loadbufferx = Library("garrysmod/bin/lua_shared").GetSymbol<luaL_loadbufferx_proto>("luaL_loadbufferx");
	luaL_loadbufferx = (luaL_loadbufferx_proto)DetourFunction((PBYTE)luaL_loadbufferx, (PBYTE)&luaL_loadbufferx_hook);
	return 0;
}

GMOD_MODULE_CLOSE() {
    return 0;
}