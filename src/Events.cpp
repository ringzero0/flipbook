#include "Events.hpp"
#include <iostream>
#include "States.hpp"


bool isDragging = false;
sf::Vector2f startPos;
sf::RectangleShape selectionBox;




void handleEvent(sf::Event &event, sf::RenderWindow &window) {

	

	if (event.is<sf::Event::Closed>()) {
		window.close();
		for (auto framedur : *currentState.nCanvas) {
			std::cout << framedur.frameTime << std::endl;
		}
	}
	//else if (event.is<sf::Event::Resized>())
	//{
	//	currentState.view.setCenter(currentState.canvas->getCanvasPosition());
	//	currentState.view.setSize(sf::Vector2f(window.getSize()));
	//	//currentState.view.zoom(100);
	//	//sf::View view(, );
	//	window.setView(currentState.view);
	//	std::cout << "resized window get size: " << window.getSize().x << " " << window.getSize().y << std::endl;
	//	//currentState.canvasPosition = 
	//}
	// 
	 else if (event.is<sf::Event::Resized>())
	{
		resize(window);
	}

	//else if (event.is<sf::Event::Resized>()) {
	//	float windowW = static_cast<float>(window.getSize().x);
	//	float windowH = static_cast<float>(window.getSize().y);

	//	float canvasW = static_cast<float>(currentState.width);
	//	float canvasH = static_cast<float>(currentState.height);

	//	float scaleX = windowW / canvasW;
	//	float scaleY = windowH / canvasH;
	//	float scale = std::min(scaleX, scaleY);

	//	currentState.view.setSize({ canvasW, canvasH });
	//	currentState.view.zoom(1.f / scale);
	//	currentState.view.setCenter(currentState.canvas->getCanvasPosition());

	//	window.setView(currentState.view);
	//}




	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && event.is<sf::Event::MouseWheelScrolled>()) {

		float mDelta = event.getIf<sf::Event::MouseWheelScrolled>()->delta;
		currentState.view = window.getView();
		if (mDelta > 0) {
			currentState.view.zoom(0.8);
		}
		else {
			currentState.view.zoom(1.2);

		}
		window.setView(currentState.view);

	}
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
	//	currentState.canvas->popFromUndoStack();
	//}
	//else {
	//	handleUndo(event, window);
	//}
	//else{
	//	const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();

	//		if (keyPressed->control && keyPressed->scancode == sf::Keyboard::Scan::Z) {
	//			currentState.canvas->popFromUndoStack();

	//		}
	//	
	//}




	static bool dragging = false;
	static sf::Vector2f grabOffset;
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		if (!dragging)
		{
			dragging = true;
			grabOffset = currentState.canvas->getCanvasPosition() - worldPos;
		}
		//currentState.canvasPosition(worldPos + grabOffset
		currentState.canvasPosition = worldPos + grabOffset;
		currentState.canvas->SetCanvasPosition(currentState.canvasPosition);

	}
	else
	{
		dragging = false;

	
		switch (currentState.pmode)
		{
		case DRAW:
			handleDraw(event, window);
			break;
		case MOVE:
			handleMove(event, window);
			break;
		case SELECT:
			handleSelect(event, window);
			break;

		case FILL:
			handleFill(event, window);
			break;

		case ERASE:
			handleErase(event, window);
			break;

		}
	
	}
}

void handleMove(sf::Event& event, sf::RenderWindow& window) {

}


void handleUndo(sf::Event& event, sf::RenderWindow& window) {
	
		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {


			if (keyPressed->control && keyPressed->scancode == sf::Keyboard::Scan::Z) {

				std::cout << "ctrlz" << std::endl;
				currentState.canvas->popFromUndoStack();

			}
		}
		
}



void handleSelect(sf::Event& event, sf::RenderWindow& window) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
		sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

		if (!isDragging) {
			isDragging = true;
			startPos = mouseWorld;

			selectionBox.setFillColor(sf::Color(0, 120, 215, 50));
			selectionBox.setPosition(startPos);
			selectionBox.setSize({ 0.f, 0.f });
		}
		else {
			sf::Vector2f size(mouseWorld.x - startPos.x, mouseWorld.y - startPos.y);
			selectionBox.setSize({ std::abs(size.x), std::abs(size.y) });

			sf::Vector2f topLeft(
				std::min(startPos.x, mouseWorld.x),
				std::min(startPos.y, mouseWorld.y)
			);
			selectionBox.setPosition(topLeft);
		}
	}
	else {
		isDragging = false;

		sf::Vector2f originOffset = currentState.canvas->getCanvasOrigin();
		sf::Vector2f localPos = selectionBox.getPosition() - (currentState.canvas->getCanvasPosition() - originOffset);
		std::cout << "size: " << selectionBox.getSize().x << " " << selectionBox.getSize().y << std::endl;
		std::cout << "position: " << localPos.x << " " << localPos.y << std::endl;

	}
}


void drawSelection(sf::RenderWindow& window) {
	if (isDragging) {
		window.draw(selectionBox);
	}
}



void handleErase(sf::Event& event, sf::RenderWindow& window) {
	drawPixels(event, window, sf::Color::Transparent);
}

void handleFill(sf::Event& event, sf::RenderWindow& window) {

	sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
	sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);
	sf::Vector2f originOffset = currentState.canvas->getCanvasOrigin();
	sf::Vector2f localPos = mouseWorld - (currentState.canvas->getCanvasPosition() - originOffset);


	//std::cout << localPos.x << localPos.y << std::endl;

	//sf::Color w;

	//if (currentState.canvas->canvasImage.getPixel()) {
	//	w = currentState.canvas->canvasImage.getPixel();
	//}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

		std::cout << localPos.x << " " << localPos.y << std::endl;

		/*try {*/

			if ((localPos.x > 0 && localPos.y > 0) && (localPos.x < currentState.canvas->getCanvasSize().x && localPos.y < currentState.canvas->getCanvasSize().y)) {
				fill(currentState.canvas->canvasImage, sf::Vector2u(localPos).x, sf::Vector2u(localPos).y, currentState.canvas->canvasImage.getPixel(sf::Vector2u(localPos)), currentState.selectedColor);

			}


		/*}
		catch (...) {
			std::cout << "left click bug or pixel set out of canvas width/height " << std::endl;
		}*/
	}

	
}


//recurrsion warning os stack size, do not use it for 20 or more
//void fill(sf::Image& img, unsigned int x, unsigned int y, sf::Color targetColor, sf::Color replacementColor) {
//	if (x < 0 || y < 0 || x >= img.getSize().x || y >= img.getSize().y) return;
//	if (img.getPixel({x,y}) != targetColor) return;
//	if (img.getPixel({x,y}) == replacementColor) return;
//
//	img.setPixel({ x, y }, replacementColor);
//
//	fill(img, x + 1, y, targetColor, replacementColor);
//	fill(img, x - 1, y, targetColor, replacementColor);
//	fill(img, x, y + 1, targetColor, replacementColor);
//	fill(img, x, y - 1, targetColor, replacementColor);
//}


void fill(sf::Image& img, unsigned int x, unsigned int y, sf::Color targetColor, sf::Color replacementColor) {
	if (targetColor == replacementColor) return;

	std::queue<std::pair<int, int>> q;
	q.push({ x, y });

	while (!q.empty()) {
		auto [cx, cy] = q.front();
		q.pop();
		//q.

		if (cx < 0 || cy < 0 || cx >= img.getSize().x|| cy >= img.getSize().y) continue;
		if (img.getPixel(sf::Vector2u(cx, cy)) != targetColor) continue;

		img.setPixel(sf::Vector2u( cx, cy ), replacementColor);

		q.push({ cx + 1, cy });
		q.push({ cx - 1, cy });
		q.push({ cx, cy + 1 });
		q.push({ cx, cy - 1 });
	}
}


void resize(sf::RenderWindow& window) {
	sf::Vector2u canvasSize = currentState.canvas->canvasImage.getSize();
	if (canvasSize.x == 0 || canvasSize.y == 0) return; // avoid division by zero

	sf::Vector2u windowSize = window.getSize();

	float windowAspect = static_cast<float>(windowSize.x) / windowSize.y;
	float canvasAspect = static_cast<float>(canvasSize.x) / canvasSize.y;

	sf::Vector2f viewSize;

	// Keep aspect ratio: choose width or height as reference
	if (windowAspect > canvasAspect)
	{
		// Window is wider than canvas -> fit height
		viewSize.y = static_cast<float>(canvasSize.y);
		viewSize.x = canvasSize.y * windowAspect;
	}
	else
	{
		// Window is taller -> fit width
		viewSize.x = static_cast<float>(canvasSize.x);
		viewSize.y = canvasSize.x / windowAspect;
	}

	currentState.view.setSize(viewSize);
	currentState.view.setCenter(currentState.canvas->getCanvasPosition());
	window.setView(currentState.view);

	std::cout << "Window: " << windowSize.x << "x" << windowSize.y
		<< " | View size: " << viewSize.x << "x" << viewSize.y << std::endl;
}


void handleDraw(sf::Event& event, sf::RenderWindow& window) {
	drawPixels(event, window, currentState.selectedColor);
}


void drawPixels(sf::Event& event, sf::RenderWindow& window, sf::Color color) {
	//color.a = 255;
	static sf::Vector2f prevMousePos(-1, -1);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {


		sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
		sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);
		sf::Vector2f originOffset = currentState.canvas->getCanvasOrigin();
		sf::Vector2f localPos = mouseWorld - (currentState.canvas->getCanvasPosition() - originOffset);

		if (prevMousePos != sf::Vector2f(-1, -1)) {
			sf::Vector2f delta = localPos - prevMousePos;
			int steps = std::max(std::abs(delta.x), std::abs(delta.y));

			if (steps == 0) {
				steps = 1;
			}

			for (int i = 0; i <= steps; i++) {
				sf::Vector2f interp = prevMousePos + delta * (i / float(steps));

				if (interp.x >= 0 && interp.y >= 0 &&
					interp.x < currentState.canvas->getCanvasSize().x &&
					interp.y < currentState.canvas->getCanvasSize().y)
				{
					currentState.canvas->setPixel(sf::Vector2u(
						static_cast<unsigned int>(interp.x),
						static_cast<unsigned int>(interp.y)
					), color);
				}
			}
		}


		prevMousePos = localPos;



	}
	else {
		prevMousePos = sf::Vector2f(-1, -1);
	}
}