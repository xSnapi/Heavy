#include "Application.hpp"

#include "Asset Loader.hpp"

#include "Heavy Utilities.hpp"
#include "Heavy ImGui.hpp"

Application::Application() {
	InitWindow();
	InitAssets();
}

Application::~Application() {

}

void Application::FixedUpdate() {

}

static float pos[2] = { 400.0f, 400.0f };
static float radius = 100.0f;
void Application::Update() {
	ImGui::Begin("light");
	ImGui::DragFloat("radius", &radius, 1.0f, 0.0f, 800.0f);
	ImGui::DragFloat2("pos", pos, 1.0f, 0.0f, 800.0f);
	ImGui::End();
	
	m_light.SetPosition(sf::Vector2f(pos[0], pos[1]));
	m_light.SetRadius(radius);
}

void Application::Render() {
	sf::Sprite s(hv::TextureLibrary::Get().Res("background"));
	s.setColor(sf::Color(100, 100, 100));
	m_window.draw(s);

	m_window.draw(m_light);
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