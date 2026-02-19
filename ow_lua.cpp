#include "ow_lua.h"
#include <iostream>

OW_Lua::OW_Lua() { L = nullptr; }
OW_Lua::~OW_Lua() 
{ 
    if(L) lua_close(L); 
}

bool OW_Lua::init()
{
    L = luaL_newstate();
    if (!L) return false;
    luaL_openlibs(L);
    return true;
}

bool OW_Lua::loadScript(const std::string& path)
{
    if (luaL_dofile(L, path.c_str()) != LUA_OK)
    {
        std::cerr << "Lua Error: " << lua_tostring(L, -1) << "\n";
        return false;
    }
    return true;
}

void OW_Lua::callFunction(const std::string& funcName)
{
    lua_getglobal(L, funcName.c_str());
    if(lua_pcall(L, 0, 0, 0) != LUA_OK)
    {
        std::cerr << "Lua call error: " << lua_tostring(L, -1) << "\n";
    }
}

void OW_Lua::setInt(const std::string& name, int value)
{
    lua_pushinteger(L, value);
    lua_setglobal(L, name.c_str());
}

int OW_Lua::getInt(const std::string& name)
{
    lua_getglobal(L, name.c_str());
    if (!lua_isinteger(L, -1)) { lua_pop(L, 1); return 0; }
    int val = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return val;
}
