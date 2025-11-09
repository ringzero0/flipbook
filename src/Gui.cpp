#include "Gui.hpp"

Gui::Gui(sf::RenderWindow& window) : window(window)
{
	ImGui::SFML::Init(window);
}



Gui::~Gui()
{
	ImGui::SFML::Shutdown();
}



void Gui::startUp(){
	ImGui::ShowDemoWindow();
}

void Gui::processEvents(sf::Event &event)
{
	ImGui::SFML::ProcessEvent(window, event);
}

bool Gui::wantMouse()
{
	return ImGui::GetIO().WantCaptureMouse;
}

void Gui::update(sf::Time dt)
{
	ImGui::SFML::Update(window, dt);
}

void Gui::render(){
	ImGui::SFML::Render(window);
}