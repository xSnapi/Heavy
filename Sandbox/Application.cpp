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
	
	m_Edges.emplace_back(sf::Vector2f(300.0f, 400.0f), sf::Vector2f(600.0f, 400.0f));

	m_Edges.emplace_back(sf::Vector2f(200.0f, 200.0f), sf::Vector2f(300.0f, 200.0f));
	m_Edges.emplace_back(sf::Vector2f(300.0f, 200.0f), sf::Vector2f(300.0f, 300.0f));
	m_Edges.emplace_back(sf::Vector2f(300.0f, 300.0f), sf::Vector2f(200.0f, 300.0f));
	m_Edges.emplace_back(sf::Vector2f(200.0f, 300.0f), sf::Vector2f(200.0f, 200.0f));

	m_light0.SetRadius(400.0f);
	m_light0.SetPosition(sf::Vector2f(400.0f, 300.0f));
	m_light0.SetAttenuation(0.4f);

	m_light1.SetRadius(300.0f);
	m_light1.SetPosition(sf::Vector2f(10.0f, 10.0f));
}

Application::~Application() {

}

void Application::FixedUpdate() {

}

void Application::Update() {
	if(!hv::Input::Mouse::KeyCheck(sf::Mouse::Left, true))
		m_light0.SetPosition(hv::Input::Mouse::GetRelativePosition());
	else
		m_light1.SetPosition(hv::Input::Mouse::GetRelativePosition());

	if (hv::EventDispatcher::CheckFor(sf::Event::MouseWheelScrolled)) {
		float delta = hv::EventDispatcher::GetEvent(sf::Event::MouseWheelScrolled).mouseWheelScroll.delta;
		hv::LightWorld::Get().SetLightLevel(hv::LightWorld::Get().GetLightLevel() + 0.05f * delta);
	}
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
