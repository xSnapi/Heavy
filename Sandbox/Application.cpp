#include "Application.hpp"

#include "Asset Loader.hpp"

#include "Heavy Utilities.hpp"
#include "Heavy ImGui.hpp"
#include "Light World.hpp"
#include "Event Dispatcher.hpp"
#include "Physics World.hpp"

Application::Application() {
	InitWindow();
	InitAssets();

	m_Lights.reserve(100);

	hv::LightWorld::Get().SetLightEnabled(true);
	hv::PhysicsWorld::Get().EnableDebugDraw(hv::DebugDrawType::Collider, hv::DebugDrawType::MassCenter, hv::DebugDrawType::AABB);

	cl.SetSize(sf::Vector2f(200.0f, 200.0f));
	rb.SetPosition(sf::Vector2f(400.0f, 400.0f));
	rb.SetCollider(cl);
}

Application::~Application() {

}

void Application::FixedUpdate() {

}

void Application::Update() {
	sf::Vector2f pos = hv::Input::Mouse::GetRelativePosition();

	if (hv::EventDispatcher::CheckFor(sf::Event::MouseWheelScrolled)) {
		int delta = (int)hv::EventDispatcher::GetEvent(sf::Event::MouseWheelScrolled).mouseWheelScroll.delta;

		if (delta != 0)
			for (auto& l : m_Lights)
				l.SetAttenuation(l.GetAttenuation() + 0.05f * delta);
	}

	if (hv::Input::Mouse::KeyCheck(sf::Mouse::Left))
		m_Lights.emplace_back(pos, 200.0f);

	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::W, true))
		hv::LightWorld::Get().SetLightLevel(hv::LightWorld::Get().GetLightLevel() + 0.05f);

	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::S, true))
		hv::LightWorld::Get().SetLightLevel(hv::LightWorld::Get().GetLightLevel() - 0.05f);

	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::Z))
		m_Lights.pop_back();
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