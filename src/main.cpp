#include "imgui.h"
#include "imgui-SFML.h"

#include "Gui.hpp"


#include <iostream>
#include <SFML/Graphics.hpp>
#include "States.hpp"

#include "Canvas.hpp"
#include "Events.hpp"
#include "canvasStates.hpp"

#define WIDTH 800
#define HEIGHT 600




sf::Time accumulator = sf::Time::Zero;


int main() {

	static sf::Clock globalClock;
	static sf::Time accumulator = sf::Time::Zero;
	static bool justStartedPlaying = false;



	int currentFrame = currentState.currentFrame;




	sf::RenderWindow window(sf::VideoMode({ WIDTH,HEIGHT }), "Flipbook");
	// ImGui::SFML::Init(window);

	window.setVerticalSyncEnabled(true);






	std::vector<Canvas> nCanvas;
	currentState.nCanvas = &nCanvas;
	nCanvas.reserve(1000);


	Gui gui(window);


	nCanvas.emplace_back(&window, currentState.width, currentState.height);
	nCanvas.emplace_back(&window, currentState.width, currentState.height);

	bool started = true;
	sf::Clock deltaClock;
	while (window.isOpen()) {


		currentFrame = currentState.currentFrame;
		currentState.canvas = &nCanvas[currentFrame];

		if (started) {
			resize(window);
			started = false;
		}


		while (std::optional event = window.pollEvent())
		{
			

			sf::Vector2i mousePos = sf::Mouse::getPosition(window);



			gui.processEvents(*event);

			if (!gui.wantMouse()) {

				handleEvent(*event, window);
			}
			
			handleUndo(*event, window);





		}
		gui.update(deltaClock.restart());
		gui.startUp();

		//if (animationState.isPlaying) {
		//	if (animationState.clock.getElapsedTime().asMilliseconds() >=  ((*currentState.nCanvas)[currentState.currentFrame].frameTime)) {
		//		
		//		//canvas->setPixels(frames[currentFrame]);
		//		currentState.currentFrame = (currentState.currentFrame + 1) % currentState.nCanvas->size();
		//		//currentState.canvas = &((*currentState.nCanvas)[currentState.currentFrame]);
		//		gui.highlightCurrentFrame(currentState.currentFrame);
		//		animationState.clock.restart();
		//	}
		//}


		if (animationState.isPlaying) {

			if (justStartedPlaying) {
				globalClock.restart();
				accumulator = sf::Time::Zero;
				justStartedPlaying = false;
			}


			sf::Time dt = globalClock.restart();
			accumulator += dt;


			if (accumulator.asSeconds() > 1.0f)
				accumulator = sf::seconds(1.0f);


			float frameDuration = (*currentState.nCanvas)[currentState.currentFrame].frameTime / 1000.f;
			if (frameDuration <= 0.0f)
				frameDuration = 0.1f;


			if (accumulator.asSeconds() >= frameDuration) {
				accumulator -= sf::seconds(frameDuration);
				currentState.currentFrame = (currentState.currentFrame + 1) % currentState.nCanvas->size();
				//gui.highlightCurrentFrame(currentState.currentFrame);
			}
		}
		else {

			justStartedPlaying = true;
		}





		currentState.canvas->pushToUndoStack();





		window.clear(sf::Color(24, 24, 24));
		//nCanvas[currentFrame].Draw();
		if (currentState.canvasPosition != sf::Vector2f({ 3,3 })) {
			currentState.canvas->SetCanvasPosition(currentState.canvasPosition);
		}




		currentState.canvas->Draw(currentState.onion, currentState.grid);
		drawSelection(window);

		//gui.draw();

		// ImGui::SFML::Render(window);
		gui.render();

		window.display();

		if (currentState.onion && currentState.currentFrame != 0) {

			size_t size = currentState.nCanvas->size();
			size_t prevFrame = (currentState.currentFrame + size - 1) % size;
			currentState.canvas->setOnion((*currentState.nCanvas)[prevFrame].canvasImage);
		}

	}
	// ImGui::SFML::Shutdown();
	gui.~Gui();

	return 0;
}

