#pragma once
#include <SFML/Graphics.hpp>


#include "Canvas.hpp"

//Canvas;

enum primaryModes {
	DRAW,
	SELECT,
	FILL,
	MOVE,
	ERASE
};

typedef struct States {
	sf::View				view;
	int						currentFrame;
	primaryModes			pmode;
	Canvas					*canvas;
	sf::Color				selectedColor;
	std::vector<Canvas>		*nCanvas;
	uint16_t				width;
	uint16_t				height;
	sf::Vector2f			canvasPosition;
	sf::Color				canvasBackgroundColor;
	bool					onion;
	bool					grid;

};


typedef struct animationStates {
	bool isPlaying;
	sf::Clock clock;
	
};

typedef struct color {
	std::vector<sf::Color> colorpalette;
};

extern States currentState;
extern animationStates animationState;
extern color colors;

