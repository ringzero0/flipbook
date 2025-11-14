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
    lua_register(L, "moveRegion", canvas_moveRegion);
    lua_register(L, "copyRegion", canvas_copyRegion);


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


int LuaEmbed::canvas_moveRegion(lua_State* L) {
    if (!currentState.canvas)
        return luaL_error(L, "Canvas not initialized");

    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int dx = luaL_checkinteger(L, 5);
    int dy = luaL_checkinteger(L, 6);

    auto canvasSize = currentState.canvas->getCanvasSize();

    if (x < 0 || y < 0 || w <= 0 || h <= 0 || dx < 0 || dy < 0)
        return luaL_error(L, "Coordinates and sizes must be non-negative and non-zero");
    if (dx < 0 || dy < 0 || dx + w > static_cast<int>(canvasSize.x) || dy + h > static_cast<int>(canvasSize.y))
        return luaL_error(L, "Destination rectangle exceeds canvas size");

    if (dx < 0 || dy < 0 || dx + w > static_cast<int>(canvasSize.x) || dy + h > static_cast<int>(canvasSize.y))
        return luaL_error(L, "Destination rectangle exceeds canvas size");


    moveRegion(x, y, w, h, dx, dy);
    return 0;
}



int LuaEmbed::canvas_copyRegion(lua_State* L) {
    if (!currentState.canvas)
        return luaL_error(L, "Canvas not initialized");

    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int dx = luaL_checkinteger(L, 5);
    int dy = luaL_checkinteger(L, 6);

    auto canvasSize = currentState.canvas->getCanvasSize();

    if (x < 0 || y < 0 || w <= 0 || h <= 0 || dx < 0 || dy < 0)
        return luaL_error(L, "Coordinates and sizes must be non-negative and non-zero");
    if (dx < 0 || dy < 0 || dx + w > static_cast<int>(canvasSize.x) || dy + h > static_cast<int>(canvasSize.y))
        return luaL_error(L, "Destination rectangle exceeds canvas size");

    if (dx < 0 || dy < 0 || dx + w > static_cast<int>(canvasSize.x) || dy + h > static_cast<int>(canvasSize.y))
        return luaL_error(L, "Destination rectangle exceeds canvas size");


    copyRegion(x, y, w, h, dx, dy);
    return 0;
}




void LuaEmbed::moveRegion(int topx, int topy, int bx, int by, int dx, int dy) {
    // int width = std::abs(topx - bx);
    // int height = std::abs(topy - by);
    //
    // sf::Image temp({static_cast<unsigned int>(width),static_cast<unsigned int>(height)}, sf::Color::Transparent);
    //
    // sf::IntRect rect({topx,topy},{width,height});
    //
    // temp.copy()
    //
    // currentState.canvas->canvasImage.copy();

    if (!currentState.canvas)
        return;



    sf::Image &img = currentState.canvas->canvasImage;

    // Compute rectangle width & height
    int width  = std::abs(bx - topx) +1;
    int height = std::abs(by - topy) +1;

    if (width <= 0 || height <= 0) return;

    // Normalized top-left corner
    int left   = std::min(topx, bx);
    int top    = std::min(topy, by);

    sf::IntRect rect({left, top}, {width, height});

    // Create temp image to store copied region
    sf::Image temp({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}, sf::Color::Transparent);

    // Copy region into temp
    temp.copy(img, {0, 0}, rect, false);

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            int cx = left + x;
            int cy = top + y;

            if (cx >= 0 && cy >= 0 && cx < img.getSize().x && cy < img.getSize().y)
                img.setPixel({static_cast<unsigned int>(cx), static_cast<unsigned int>(cy)}, sf::Color::Transparent);
        }

    // Paste at new location
    int newLeft = dx;
    int newTop  = dy;


    img.copy(temp, {static_cast<unsigned int>(newLeft), static_cast<unsigned int>(newTop)}, sf::IntRect({0, 0}, {width, height}), false);

}


void LuaEmbed::copyRegion(int topx, int topy, int bx, int by, int dx, int dy) {
    // int width = std::abs(topx - bx);
    // int height = std::abs(topy - by);
    //
    // sf::Image temp({static_cast<unsigned int>(width),static_cast<unsigned int>(height)}, sf::Color::Transparent);
    //
    // sf::IntRect rect({topx,topy},{width,height});
    //
    // temp.copy()
    //
    // currentState.canvas->canvasImage.copy();

    if (!currentState.canvas)
        return;



    sf::Image &img = currentState.canvas->canvasImage;

    int width  = std::abs(bx - topx) +1;
    int height = std::abs(by - topy) +1;

    if (width <= 0 || height <= 0) return;

    int left   = std::min(topx, bx);
    int top    = std::min(topy, by);

    sf::IntRect rect({left, top}, {width, height});

    sf::Image temp({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}, sf::Color::Transparent);

    temp.copy(img, {0, 0}, rect, false);



    // Paste at new location
    int newLeft = dx;
    int newTop  = dy;


    img.copy(temp, {static_cast<unsigned int>(newLeft), static_cast<unsigned int>(newTop)}, sf::IntRect({0, 0}, {width, height}), false);

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



