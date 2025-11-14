//
// Created by Praveen P on 14-11-2025.
//

#ifndef PIKZEL_LUAEMBED_H
#define PIKZEL_LUAEMBED_H
#include "include/lua.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include "States.hpp"


class LuaEmbed {

    lua_State* L;

public:
    LuaEmbed();
    ~LuaEmbed();
    int luaDoString(const char* code);
    void luaPopStack(int n);
    lua_State* getLuaState();
    const char* luaToString(signed int n);
    static void moveRegion(int topx, int topy, int bx, int by, int dx, int dy);
    static void copyRegion(int topx, int topy, int bx, int by, int dx, int dy);
    static int canvas_moveRegion(lua_State* L);
    static int canvas_copyRegion(lua_State* L);

    static int canvas_setPixel(lua_State* L);
    static int canvas_getPixel(lua_State* L);
    static int canvas_getWidth(lua_State* L);
    static int canvas_getHeight(lua_State* L);


};


#endif //PIKZEL_LUAEMBED_H