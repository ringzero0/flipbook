#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>

#include <misc/cpp/imgui_stdlib.h>


#include "FileUtils.hpp"
#include "pxrt.hpp"
#include "Events.hpp"
#include "include/nfd.hpp"
#include "misc.h"
#include "TextEditor.h"

#include "LuaEmbed.h"
#include "promptReq.h"
#include <thread>


#include "FileUtils.hpp"
#include "States.hpp"



class Gui
{
	
	sf::RenderWindow& window;
	inline static bool showColorPicker = false;
	inline static bool showStartupWindow = false;
	TextEditor editor;
	LuaEmbed *lua;


public:

	Gui(sf::RenderWindow& window);
	~Gui();

	void init();
	void startUp();

	void processEvents(sf::Event &event);

	bool wantMouse();

	void update(sf::Time dt);

	void render();

	void menuBar();
	void drawLeftPanel();
	void drawColorSelector();
	void drawAnimationPanel();
	void drawToolBar();
	void drawColorShadesPanel();
	void codeWindow();



};

