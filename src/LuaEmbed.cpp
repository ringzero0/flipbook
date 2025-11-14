//
// Created by Praveen P on 14-11-2025.
//

#include "LuaEmbed.h"

LuaEmbed::LuaEmbed(){
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "setPixel", canvas_setPixel);
    lua_register(L, "getPixel", canvas_getPixel);
    lua_register(L, "getWidth", canvas_getWidth);
    lua_register(L, "getHeight", canvas_getHeight);


}

int LuaEmbed::canvas_getWidth(lua_State* L) {

    if (!currentState.canvas) {
        return luaL_error(L, "Canvas is not initialized");
    }

    int width = currentState.canvas->getCanvasSize().x;

    lua_pushinteger(L, width);



    return 1;
}


int LuaEmbed::canvas_getHeight(lua_State* L) {

    if (!currentState.canvas) {
        return luaL_error(L, "Canvas is not initialized");
    }

    int height = currentState.canvas->getCanvasSize().y;

    lua_pushinteger(L, height);




    return 1;
}



int LuaEmbed::canvas_setPixel(lua_State* L) {
     int lx = luaL_checkinteger(L, 1);
     int ly = luaL_checkinteger(L, 2);

    if (lx < 0 || ly < 0)
    {
        return luaL_error(L, "x and y must be non-negative integers");
    }

    unsigned int x = static_cast<unsigned int>(lx);
    unsigned int y = static_cast<unsigned int>(ly);

    if (x >= currentState.canvas->getCanvasSize().x || y >= currentState.canvas->getCanvasSize().y ) {
        return luaL_error(L, "x or y greater than canvas size");
    }

    // Read color table {r=.., g=.., b=..}, alpha = 255
    sf::Color color(255, 255, 255, 255);
    if (lua_istable(L, 3)) {
        lua_getfield(L, 3, "r");
        color.r = (uint8_t)luaL_checkinteger(L, -1); lua_pop(L, 1);
        lua_getfield(L, 3, "g");
        color.g = (uint8_t)luaL_checkinteger(L, -1); lua_pop(L, 1);
        lua_getfield(L, 3, "b");
        color.b = (uint8_t)luaL_checkinteger(L, -1); lua_pop(L, 1);
    }

    currentState.canvas->setPixel({x,y}, color);

    std::cout << "Set pixel at (" << x << "," << y << ") color("
              << (int)color.r << "," << (int)color.g << "," << (int)color.b << ")\n";

    return 0;
}

int LuaEmbed::canvas_getPixel(lua_State* L) {
    if (!currentState.canvas) {
        return luaL_error(L, "Canvas is not initialized");
    }

    int lx = luaL_checkinteger(L, 1);
    int ly = luaL_checkinteger(L, 2);

    if (lx < 0 || ly < 0) {
        return luaL_error(L, "x and y must be non-negative integers");
    }

    unsigned int x = static_cast<unsigned int>(lx);
    unsigned int y = static_cast<unsigned int>(ly);

    if (x >= currentState.canvas->getCanvasSize().x || y >= currentState.canvas->getCanvasSize().y) {
        return luaL_error(L, "x or y greater than canvas size");
    }

    sf::Color color = currentState.canvas->getPixel({x, y});

    lua_newtable(L);

    lua_pushinteger(L, color.r);
    lua_setfield(L, -2, "r");

    lua_pushinteger(L, color.g);
    lua_setfield(L, -2, "g");

    lua_pushinteger(L, color.b);
    lua_setfield(L, -2, "b");

    lua_pushinteger(L, color.a);
    lua_setfield(L, -2, "a");

    return 1;
}



// void Canvas::moveRegion(const sf::IntRect& region, int dx, int dy) {
//     sf::Image temp(region.size, sf::Color::Transparent);
//
//
//     // Copy the region into temp
//     for (int x = 0; x < region.size.x; ++x) {
//         for (int y = 0; y < region.size.y; ++y) {
//             temp.setPixel(x, y, getPixel({region.left + x, region.top + y}));
//         }
//     }
//
//     // Clear original region
//     for (int x = 0; x < region.width; ++x) {
//         for (int y = 0; y < region.height; ++y) {
//             setPixel({region.left + x, region.top + y}, sf::Color(0,0,0,0)); // or background color
//         }
//     }
//
//     // Paste temp at new position
//     for (int x = 0; x < region.width; ++x) {
//         for (int y = 0; y < region.height; ++y) {
//             int newX = region.left + x + dx;
//             int newY = region.top + y + dy;
//
//             if (newX >= 0 && newY >= 0 && newX < getCanvasSize().x && newY < getCanvasSize().y)
//                 setPixel({newX, newY}, temp.getPixel(x, y));
//         }
//     }
// }



LuaEmbed::~LuaEmbed(){
    lua_close(L);
}




int LuaEmbed::luaDoString(const char *code) {

    int strret = luaL_dostring(L,code);
    // lua_settop(L, 0); // resetting the stack
    return strret;

}

lua_State *LuaEmbed::getLuaState() {
    return L;
}

void LuaEmbed::luaPopStack(int n) {
    lua_pop(L,n);
}

const char* LuaEmbed::luaToString(signed int n) {
    return lua_tostring(L,n);
}



