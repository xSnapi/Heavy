#include "Application.hpp"

#include "Asset Loader.hpp"
#include "Heavy ImGui.hpp"

Application::Application() {
	InitWindow();
	InitAssets();

	m_campfire.FrameWidth = 500;
	m_campfire.FrameHeight = 500;
	m_campfire.Speed = 0.25f;
	m_campfire.Reversed = false;

	m_campfire.Create(10, 1, 0, &m_campfireSprite, &hv::TextureLibrary::Get().Res("campfire"));
}

Application::~Application() {

}

void Application::FixedUpdate() {

}

void Application::Update() {
	m_animator.Play(m_campfire);

	ImGui::Begin("Window");
	m_campfire.GetImGuiInfo("campfire");
	ImGui::End();

	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::Space))
		m_campfire.Reversed = !m_campfire.Reversed;
}

void Application::Render() {
	m_renderer.draw(sf::Sprite(hv::TextureLibrary::Get().Res("background")));
	m_renderer.draw(m_campfireSprite);
}

void Application::InitAssets() {
	hv::AssetLoader::Get().AddResource("background", "res/bricks.png", hv::AssetType::Texture);
	hv::AssetLoader::Get().AddResource("campfire", "res/numeras.png", hv::AssetType::Texture);
	hv::AssetLoader::Get().Sync();
}

void Application::InitWindow() {
	sf::VideoMode videoMode;

	videoMode.width = 800;
	videoMode.height = 800;

	sf::ContextSettings contextSettings;
	//contextSettings.antialiasingLevel = 8;

	m_window.create(videoMode, "Sandbox", sf::Style::Close, contextSettings);

	SetFrameLimit(144);
}
