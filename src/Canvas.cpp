#include "Canvas.hpp"



Canvas::Canvas(sf::RenderWindow* window, uint16_t width, uint16_t height) :
	window(window),
	canvasImage({ width, height }, sf::Color::Transparent),
	canvasBackground({ width, height }, sf::Color::White),
	onion({ width, height }, sf::Color::Transparent),
	canvasImageTexture(canvasImage),
	onionTexture(onion),
	canvasBackgroundTexture(canvasBackground),
	canvasImageSprite(canvasImageTexture),
	canvasBackgroundSprite(canvasBackgroundTexture),
	onionSprite(onionTexture)

{
	
	sharedOrigin = { canvasImageSprite.getGlobalBounds().getCenter().x, canvasImageSprite.getGlobalBounds().getCenter().y };
	canvasImageSprite.setOrigin(sharedOrigin);
	canvasBackgroundSprite.setOrigin(sharedOrigin);
	onionSprite.setOrigin(sharedOrigin);
	canvasPosition = sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2);
	SetCanvasPosition(canvasPosition);
	
}

Canvas::Canvas(sf::RenderWindow* window, uint16_t width, uint16_t height, sf::Vector2f canvasPosition) :
	window(window),
	canvasImage({ width, height }, sf::Color::Transparent),
	canvasBackground({ width, height }, sf::Color::White),
	onion({ width, height }, sf::Color::Transparent),
	canvasImageTexture(canvasImage),
	onionTexture(onion),
	canvasBackgroundTexture(canvasBackground),
	canvasImageSprite(canvasImageTexture),
	canvasBackgroundSprite(canvasBackgroundTexture),
	onionSprite(onionTexture)

{

	sharedOrigin = { canvasImageSprite.getGlobalBounds().getCenter().x, canvasImageSprite.getGlobalBounds().getCenter().y };
	canvasImageSprite.setOrigin(sharedOrigin);
	canvasBackgroundSprite.setOrigin(sharedOrigin);
	onionSprite.setOrigin(sharedOrigin);
	SetCanvasPosition(canvasPosition);

}



void Canvas::Draw(bool onionState, bool gridState)
{



	canvasImageTexture.update(canvasImage);
	onionTexture.update(onion);
	canvasBackgroundTexture.update(canvasBackground);

	window->draw(canvasBackgroundSprite);

	if (onionState) {
		window->draw(onionSprite);
	}


	window->draw(canvasImageSprite);

	if (gridState) {
		grid();
		window->draw(pixelGrid, sf::RenderStates(canvasImageSprite.getTransform()));
	}



}

void Canvas::Display()
{
	window->display();
}

void Canvas::SetCanvasPosition(sf::Vector2f positionCoords)
{
	
	canvasPosition = positionCoords;
	canvasImageSprite.setPosition(canvasPosition);
	canvasBackgroundSprite.setPosition(canvasPosition);
	onionSprite.setPosition(canvasPosition);

}

sf::Vector2f Canvas::getCanvasPosition()
{
	return canvasImageSprite.getPosition();
}



void Canvas::move(sf::Vector2f offset) {
	canvasImageSprite.move(offset);
}


void Canvas::setPixel(sf::Vector2u coords, sf::Color color) {
	canvasImage.setPixel(coords, color);
}


sf::Vector2u Canvas::getCanvasSize() {
	return canvasImage.getSize();
}

sf::Vector2f Canvas::getCanvasOrigin() {
	return canvasImageSprite.getOrigin();
}

void Canvas::exportCanvasImage() {
	canvasImage.saveToFile("D:/canvasimage.png");
}


void Canvas::setCanvasFrameTime(float frameTime) {
	frameTime = frameTime;
}

float Canvas::getCanvasFrameTime() {
	return frameTime;
}

const std::uint8_t* Canvas::getCanvasPixelPtr() {
	return canvasImage.getPixelsPtr();
}


void Canvas::pushToUndoStack() {
	ImageState state;
	const uint8_t* data = getCanvasPixelPtr();

	state.pixels.assign(data, data + (getCanvasSize().x * getCanvasSize().y * 4));

	if (!undoStack.empty() && undoStack.top().pixels == state.pixels)
		return;

	undoStack.push(std::move(state));

}

void Canvas::popFromUndoStack() {
	if (undoStack.empty() || undoStack.size()==1) {
		return;
	}
	undoStack.pop();

	auto state = undoStack.top();	
	canvasImage.resize(canvasImage.getSize(), state.pixels.data());
}

void Canvas::setOnion(const sf::Image &source) {

	onion.copy(source, {0,0}, {}, true);

	onionSprite.setColor(sf::Color(255, 255, 255, 100));






	//canvasBackground.createMaskFromColor(sf::Color::Blue, 255);
	//canvasBackgroundTexture.update(window);

}

void Canvas::copyOnionToCanvas(const sf::Image& source) {

	canvasImage.copy(source, { 0,0 }, {}, true);

	//onionSprite.setColor(sf::Color(255, 255, 255, 100));






	//canvasBackground.createMaskFromColor(sf::Color::Blue, 255);
	//canvasBackgroundTexture.update(window);

}

void Canvas::grid() {

	//if (!isGridOn) {

		sf::Vector2u size = getCanvasSize();

		unsigned int width = size.x;
		unsigned int height = size.y;


		pixelGrid = sf::VertexArray(sf::PrimitiveType::Lines);
		gridColor = sf::Color(0, 0, 0, 255);

		for (unsigned int x = 0; x <= width; ++x) {
			pixelGrid.append(sf::Vertex(sf::Vector2f(x, 0), gridColor));
			pixelGrid.append(sf::Vertex(sf::Vector2f(x, height), gridColor));
		}
		for (unsigned int y = 0; y <= height; ++y) {
			pixelGrid.append(sf::Vertex(sf::Vector2f(0, y), gridColor));
			pixelGrid.append(sf::Vertex(sf::Vector2f(width, y), gridColor));
		}

		//isGridOn = !isGridOn;
	//}
}


Canvas::~Canvas()
{
	
}


