#pragma once
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <string>

class OW_Lua
{
public:
    OW_Lua();
    ~OW_Lua();

    bool init();
    bool loadScript(const std::string& path);
    void callFunction(const std::string& funcName);
    void setInt(const std::string& name, int value);
    int getInt(const std::string& name);
private:
    lua_State* L = nullptr;
};
