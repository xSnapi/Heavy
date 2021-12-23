#include "Application.hpp"

#include "Asset Loader.hpp"

#include "Heavy Utilities.hpp"
#include "Heavy ImGui.hpp"

Application::Application() {
	InitWindow();
	InitAssets();

	floorC.SetSize(sf::Vector2f(400.0f, 400.0f));
	rbC.SetSize(sf::Vector2f(50.0f, 50.0f));

	floor.SetCollider(floorC);
	floor.SetPosition(sf::Vector2f(400.0f, 400.0f));
}

Application::~Application() {

}

void Application::FixedUpdate() {

}

void Application::Update() {
	if (hv::Input::Mouse::KeyCheck(sf::Mouse::Left))
		rbs.emplace_back(hv::Input::Mouse::GetRelativePosition(), hv::BodyType::Dynamic, rbC);
}

void Application::Render() {
	
}

void Application::InitAssets() {
	hv::AssetLoader::Get().AddResource("background", "res/bricks.png", hv::AssetType::Texture);
	hv::AssetLoader::Get().Sync();
}

void Application::InitWindow() {
	sf::VideoMode videoMode;

	videoMode.width  = 800;
	videoMode.height = 800;

	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	
	m_window.create(videoMode, "Sandbox", sf::Style::Close, contextSettings);

	SetFrameLimit(144);
}