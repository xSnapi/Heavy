#include "Application.hpp"

#include "Asset Loader.hpp"

#include "Heavy Utilities.hpp"
#include "Heavy ImGui.hpp"
#include "Light World.hpp"

Application::Application() {
	InitWindow();
	InitAssets();

	m_light.SetPosition(sf::Vector2f(400.0f, 400.0f));
	m_light.SetRadius(200.0f);

	m_light1.SetRadius(100.0f);

	m_light2.SetPosition(sf::Vector2f(700.0f, 650.f));
	m_light2.SetRadius(150.0f);
}

Application::~Application() {

}

void Application::FixedUpdate() {

}

static bool x = true;
void Application::Update() {
	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::Space))
		x = !x;

	hv::LightWorld::Get().Enabled = x;

	static float level = 0.0f;

	ImGui::Begin("light");
	ImGui::DragFloat("Level", &level, 0.005f, 0.0f, 1.0f);
	ImGui::End();

	hv::LightWorld::Get().SetLightLevel(level);
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

	videoMode.width  = 800;
	videoMode.height = 800;

	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	
	m_window.create(videoMode, "Sandbox", sf::Style::Close, contextSettings);

	SetFrameLimit(144);
}