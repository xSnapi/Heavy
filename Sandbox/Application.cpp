#include "Application.hpp"

#include "Asset Loader.hpp"

#include "Heavy Utilities.hpp"

Application::Application() {
	InitWindow();
	InitAssets();

	m_rigidBodyCollider.SetSize(sf::Vector2f(50.0f, 50.0f));
	m_floorCollider.SetSize(sf::Vector2f(400.0f, 25.0f));

	m_floor.SetCollider(m_floorCollider);
	m_floor.SetPosition(sf::Vector2f(400.0f, 700.0f));

	m_floorShape.setSize(sf::Vector2f(400.0f, 25.0f));
	m_floorShape.setOrigin(sf::Vector2f(200.0f, 12.5f));
	m_floorShape.setPosition(sf::Vector2f(400.0f, 700.0f));

	m_Rbs.reserve(500);

	m_emitter.SetPosition(sf::Vector2f(400.0f, 50.0f));
	m_emitter.SetDirection(1.0f, 0.0f);
	m_emitter.SetVelocity(sf::Vector2f(200.0f, 100.0f));
	m_emitter.SetHueShift(true);
}

Application::~Application() {

}

void Application::FixedUpdate() {

}

void Application::Update() {
	m_emitter.AddParticles();
	m_emitter.Update();

	sf::Vector2f pos = hv::Input::Mouse::GetRelativePosition();

	if (hv::Input::Mouse::KeyCheck(sf::Mouse::Left)) {
		m_Rbs.emplace_back(pos, hv::BodyType::Dynamic, m_rigidBodyCollider);
		m_Boxes.emplace_back(sf::Vector2f(50.0f, 50.0f));
		m_Boxes.back().setOrigin(sf::Vector2f(25.0f, 25.0f));
		m_Boxes.back().setPosition(pos);
	}

	for (size_t i = 0; i < m_Rbs.size(); i++) {
		if (m_Rbs[i].GetPosition().y > 850.0f ||
			m_Rbs[i].ContainsPoint(pos) && hv::Input::Mouse::KeyCheck(sf::Mouse::Right)) {
			m_Rbs.erase(m_Rbs.begin() + i);
			m_Boxes.erase(m_Boxes.begin() + i);
		}
	}
}

void Application::Render() {
	m_window.draw(sf::Sprite(hv::TextureLibrary::Get().Res("background")));

	for (size_t i = 0; i < m_Boxes.size(); i++) {
		auto& box = m_Boxes[i];
		auto& rb  = m_Rbs[i];
		
		box.setRotation(rb.GetRotation());
		box.setPosition(rb.GetPosition());

		box.setFillColor(hv::HueShift(sf::Color(255, 0, 0), m_clock.getElapsedTime().asSeconds()));

		m_window.draw(box, &hv::ShaderLibrary::Get().Res("gradient"));
	}

	m_floorShape.setFillColor(hv::HueShift(sf::Color(255, 0, 0), -m_clock.getElapsedTime().asSeconds()));
	m_window.draw(m_floorShape, &hv::ShaderLibrary::Get().Res("gradient"));

	m_window.draw(m_emitter);
}

void Application::InitAssets() {
	hv::AssetLoader::Get().AddResource("background", "res/sky.png", hv::AssetType::Texture);
	hv::AssetLoader::Get().AddResource("gradient", "res/gradientFrag.shader", "res/gradientVert.shader", hv::AssetType::Shader);
	hv::AssetLoader::Get().Sync();
}

void Application::InitWindow() {
	sf::VideoMode videoMode;

	videoMode.width  = 800;
	videoMode.height = 800;

	sf::ContextSettings contextSettings;
	
	m_window.create(videoMode, "Sandbox", sf::Style::Close, contextSettings);

	SetFrameLimit(144);
}