#include "Application.hpp"

#include "Asset Loader.hpp"
#include "Heavy ImGui.hpp"

#include "Camera.hpp"

Application::Application() {
	InitWindow();
	InitAssets();
	m_renderer.ClearColor = sf::Color(255, 255, 255);
	//hv::LightWorld::Get().SetLightEnabled(true);
	//hv::LightWorld::Get().SetShadowEnabled(true);
	//hv::LightWorld::Get().EnableDebugDraw(hv::DebugDrawType::LightEdge);
	hv::LightWorld::Get().SetLightLevel(0.2f);

	ed.Set(sf::Vector2f(500.0f, 400.0f), sf::Vector2f(700.0f, 400.0f));

	l0.SetRadius(400.0f);
	l0.SetPosition(sf::Vector2f(400.0f, 400.0f));

	//l1.SetPosition(sf::Vector2f(600.0f, 260.0f));
	//l1.SetRadius(300.0f);
	//l1.SetAttenuation(0.3f);

	edges.emplace_back(sf::Vector2f(150.0f, 350.0f), sf::Vector2f(150.0f, 400.0f));

	edges.emplace_back(sf::Vector2f(150.0f, 430.0f), sf::Vector2f(150.0f, 480.0f));

	edges.emplace_back(sf::Vector2f(150.0f, 510.0f), sf::Vector2f(150.0f, 560.0f));

	edges.emplace_back(sf::Vector2f(150.0f, 590.0f), sf::Vector2f(150.0f, 640.0f));


	edges.emplace_back(sf::Vector2f(250.0f, 350.0f), sf::Vector2f(250.0f, 400.0f));

	edges.emplace_back(sf::Vector2f(250.0f, 430.0f), sf::Vector2f(250.0f, 480.0f));

	edges.emplace_back(sf::Vector2f(250.0f, 510.0f), sf::Vector2f(250.0f, 560.0f));

	edges.emplace_back(sf::Vector2f(250.0f, 590.0f), sf::Vector2f(250.0f, 640.0f));

	edges.emplace_back(sf::Vector2f(150.0f, 150.0f), sf::Vector2f(250.0f, 150.0f));
	edges.emplace_back(sf::Vector2f(250.0f, 150.0f), sf::Vector2f(250.0f, 250.0f));
	edges.emplace_back(sf::Vector2f(250.0f, 250.0f), sf::Vector2f(150.0f, 250.0f));
	edges.emplace_back(sf::Vector2f(150.0f, 250.0f), sf::Vector2f(150.0f, 180.0f));

	edges.emplace_back(sf::Vector2f(300.0f, 100.0f), sf::Vector2f(400.0f, 100.0f));
	edges.emplace_back(sf::Vector2f(400.0f, 100.0f), sf::Vector2f(400.0f, 200.0f));
	edges.emplace_back(sf::Vector2f(400.0f, 200.0f), sf::Vector2f(300.0f, 200.0f));
	edges.emplace_back(sf::Vector2f(300.0f, 200.0f), sf::Vector2f(300.0f, 100.0f));
}

Application::~Application() {

}

void Application::FixedUpdate() {

}

void Application::Update() {
	sf::Vector2f pos = hv::Input::Mouse::GetRelativePosition();

	if (hv::EventDispatcher::CheckFor(sf::Event::MouseWheelScrolled)) {
		float delta = hv::EventDispatcher::GetEvent(sf::Event::MouseWheelScrolled).mouseWheelScroll.delta;

		hv::Camera::Get().Zoom(delta < 0.0f ? 1.5f : 0.5f);
	}

	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::B, true))
		hv::Camera::Get().Zoom(1.0f);
	//else
		//l1.SetPosition(pos);

	static float lightPower = 1.0f;

	{
		hv::ImGuiWindow w("window");
		//ImGui::DragFloat("power", &lightPower, 0.05f, 0.0f, 10.0f);
		l0.DisplayImGuiInfo("l0");
		//l1.DisplayImGuiInfo("l1");
	}

	sf::Vector2f move;

	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::W, true)) {
		move.y = -1.0f;
	}
	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::A, true)) {
		move.x = -1.0f;
	}
	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::S, true)) {
		move.y = 1.0f;
	}
	if (hv::Input::Keyboard::KeyCheck(sf::Keyboard::D, true)) {
		move.x = 1.0f;
	}

	hv::Camera::Get().Move(move * (hv::Input::Keyboard::KeyCheck(sf::Keyboard::LShift, true) ? 400.0f : 200.0f) * hv::dt);
}

void Application::Render() {
	m_renderer.draw(sf::Sprite(hv::TextureLibrary::Get().Res("background")));
	//m_renderer.draw(sf::RectangleShape(sf::Vector2f(200, 200)));
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
	contextSettings.antialiasingLevel = 8;

	m_window.create(videoMode, "Sandbox", sf::Style::Close, contextSettings);

	SetFrameLimit(144);
}
