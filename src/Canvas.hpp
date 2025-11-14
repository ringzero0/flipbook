#pragma once
#include <iostream>
//#include "Textures.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <stack>
//#include "Textures.hpp"
//#include "canvasStates.hpp"
//#include "States.hpp"
//#include <SFML/Main.hpp>
//#include "States.hpp"

class Canvas
{


public:

	struct ImageState {
		std::vector<uint8_t> pixels;
		//std::size_t hash;
	};
	std::stack<ImageState> undoStack;

	sf::Image			canvasImage;
	sf::Texture			canvasImageTexture;
	sf::Sprite			canvasImageSprite;

	sf::Image			canvasBackground;
	sf::Texture			canvasBackgroundTexture;
	sf::Sprite			canvasBackgroundSprite;


	sf::Image			onion;
	sf::Texture			onionTexture;
	sf::Sprite			onionSprite;

	sf::RenderWindow*	window;


	std::unordered_map<std::string, sf::Texture> externalTextures;

	unsigned int					frameTime = 0;

	inline static sf::Vector2f sharedOrigin = {0.0f, 0.0f};
	inline static sf::Vector2f canvasPosition = { 0.0f, 0.0f };

	bool isOnionCopied = false;
	bool isGridOn = true;

	sf::VertexArray pixelGrid;
	sf::Color gridColor;


	Canvas(sf::RenderWindow* window, uint16_t width, uint16_t height);
	Canvas(sf::RenderWindow* window, uint16_t width, uint16_t height, sf::Vector2f canvasPosition);
	~Canvas();


	void				Draw();
	void				Draw(bool onionState, bool gridState);
	void				Display();
	void				SetCanvasPosition(sf::Vector2f positionCoords);

	sf::Vector2f		getCanvasPosition();
	sf::Vector2u		getCanvasSize();
	sf::Vector2f		getCanvasOrigin();
	void				setPixel(sf::Vector2u coords, sf::Color color);
	sf::Color			getPixel(sf::Vector2u coords);
	void				move(sf::Vector2f offset);
	void				exportCanvasImage(const std::filesystem::path& filename);
	void				setCanvasFrameTime(float frameTime);
	float				getCanvasFrameTime();
	void				pushToUndoStack();
	void				popFromUndoStack();

	void				setOnion(const sf::Image &source);
	void				copyOnionToCanvas(const sf::Image &source);

	void				grid();

	const std::uint8_t* getCanvasPixelPtr();












};

