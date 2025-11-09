#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>


class Gui
{
	
	sf::RenderWindow& window;

public:
	Gui(sf::RenderWindow& window);
	~Gui();

	void startUp();

	void processEvents(sf::Event &event);

	bool wantMouse();

	void update(sf::Time dt);

	void render();

};

