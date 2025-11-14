#pragma once
#include <SFML/Graphics.hpp>
#include <queue>


void handleEvent(sf::Event &event, sf::RenderWindow &window);

void handleDraw(sf::Event& event, sf::RenderWindow& window);
void handleMove(sf::Event& event, sf::RenderWindow& window);
void handleSelect(sf::Event& event, sf::RenderWindow& window);
void handleFill(sf::Event& event, sf::RenderWindow& window);
void handleErase(sf::Event& event, sf::RenderWindow& window);
void handleUndo(sf::Event& event, sf::RenderWindow& window);
void resize(sf::RenderWindow& window);

sf::Vector2f getMousePosOnCanvas(sf::RenderWindow& window);

void fill(sf::Image& img, unsigned int x, unsigned int y, sf::Color targetColor, sf::Color replacementColor);

void drawSelection(sf::RenderWindow& window);

void drawPixels(sf::Event& event, sf::RenderWindow& window, sf::Color color);