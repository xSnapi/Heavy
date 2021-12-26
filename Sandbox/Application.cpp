#include "Application.hpp"

#include "Asset Loader.hpp"

#include "Heavy Utilities.hpp"
#include "Heavy ImGui.hpp"
#include "Light World.hpp"
#include "Event Dispatcher.hpp"
#include "Physics World.hpp"

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 800;

Application::Application() {
	InitWindow();
	InitAssets();

	hv::LightWorld::Get().SetLightEnabled(true);
	hv::LightWorld::Get().SetShadowEnabled(true);

	l.SetRadius(300.0f);
	//l2.SetRadius(560.0f);

	edge. Set(sf::Vector2f(200.0f, 200.0f), sf::Vector2f(300.0f, 200.0f));
	edge1.Set(sf::Vector2f(300.0f, 200.0f), sf::Vector2f(300.0f, 300.0f));
	edge2.Set(sf::Vector2f(300.0f, 300.0f), sf::Vector2f(200.0f, 300.0f));
	edge3.Set(sf::Vector2f(200.0f, 300.0f), sf::Vector2f(200.0f, 200.0f));
	
	edge4.Set(sf::Vector2f(300.0f, 400.0f), sf::Vector2f(600.0f, 400.0f));
}

Application::~Application() {

}

void Application::FixedUpdate() {

}

void Application::Update() {
	sf::Vector2f pos = hv::Input::Mouse::GetRelativePosition();

	l.SetPosition(pos);

	if (hv::EventDispatcher::CheckFor(sf::Event::MouseWheelScrolled)) {
		int delta = (int)hv::EventDispatcher::GetEvent(sf::Event::MouseWheelScrolled).mouseWheelScroll.delta;

		if (delta != 0)
			l.SetAttenuation(l.GetAttenuation() + 0.05f * delta);	
	}

	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::W, true))
		hv::LightWorld::Get().SetLightLevel(hv::LightWorld::Get().GetLightLevel() + 0.05f);

	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::S, true))
		hv::LightWorld::Get().SetLightLevel(hv::LightWorld::Get().GetLightLevel() - 0.05f);

}

void Application::Render() {
	m_renderer.draw(sf::Sprite(hv::TextureLibrary::Get().Res("background")));
}

void Application::InitAssets() {
	hv::AssetLoader::Get().AddResource("background", "res/bricks.png", hv::AssetType::Texture);
	hv::AssetLoader::Get().Sync();
}

void Application::InitWindow() {
	sf::VideoMode videoMode;

	videoMode.width = 800;
	videoMode.height = 800;

	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;

	m_window.create(videoMode, "Sandbox", sf::Style::Close, contextSettings);

	SetFrameLimit(144);
}
