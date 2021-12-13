#include "Application.hpp"

#include "Asset Loader.hpp"
#include "Heavy ImGui.hpp"

#include "Ini Parser.hpp"

Application::Application() {
	InitWindow();
	InitAssets();

	//collider.AddPoints(sf::Vector2f(200.0f, 400.0f), sf::Vector2f(600.0f, 400.0f));

	m_emitter.SetPosition(sf::Vector2f(400.0f, 400.0f));
	m_emitter.SetDirection(0.0f, -1.0f);
	m_emitter.SetSize(100.0f);
	m_emitter.SetVelocity(sf::Vector2f(200.0f, 100.0f));
	m_emitter.SetMax(99);
}

Application::~Application() {

}

void Application::FixedUpdate() {
	
}

void Application::Update() {
	m_emitter.AddParticles();
	m_emitter.Update();
}

void Application::Render() {
	m_window.draw(m_emitter);
}

void Application::InitAssets() {
	hv::TextureLibrary::Get().Load("bricks", "res/bricks.png");
}

void Application::InitWindow() {
	sf::VideoMode videoMode;

	videoMode.width  = 800;
	videoMode.height = 800;

	sf::ContextSettings contextSettings;
	
	m_window.create(videoMode, "Sandbox", sf::Style::Close, contextSettings);

	SetFrameLimit(144);
}