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
    sf::RenderWindow& window;
    static sf::Image temp;



public:

    LuaEmbed(sf::RenderWindow& window);
    ~LuaEmbed();
    int luaDoString(const char* code);
    void luaPopStack(int n);
    lua_State* getLuaState();
    const char* luaToString(signed int n);
    static void moveRegion(int topx, int topy, int bx, int by, int x1, int y1);
    static void copyRegion(int topx, int topy, int bx, int by);
    static void pasteRegion(int x1, int y1);
    static int canvas_moveRegion(lua_State* L);
    static int canvas_copyRegion(lua_State* L);
    static int canvas_pasteRegion(lua_State* L);
    static int canvas_setFrameDelay(lua_State* L);

    static int canvas_setPixel(lua_State* L);
    static int canvas_getPixel(lua_State* L);
    static int canvas_getWidth(lua_State* L);
    static int canvas_getHeight(lua_State* L);
    static int canvas_setFrames(lua_State* L);
    static int canvas_setCurrentFrame(lua_State* L);

    int emplaceBack(lua_State* L);

};


#endif //PIKZEL_LUAEMBED_H