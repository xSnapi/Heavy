#include <hvpch.h>
#include "Camera.hpp"

#include "Heavy Times.hpp"
#include "Random.hpp"

namespace hv {
	Camera& Camera::Get() { return s_instance; }

	void Camera::Init(sf::RenderWindow* window) {
		m_window = window;

		Resize((sf::Vector2f)m_window->getSize());
	}

	void Camera::Update() {
		ApplyEffects();

		m_window->setView(m_view);
	}

	void Camera::Shake(float time, int power) {
		m_shakeTime = time;

		m_sc.Center = m_view.getCenter();

		m_sc.CreatePos(power);
	}

	void Camera::Move(sf::Vector2f offset) {
		m_view.move(offset);
	}

	sf::Vector2f Camera::GetCenter() const {
		return m_view.getCenter();
	}

	sf::Vector2f Camera::GetSize() const {
		return m_view.getSize();
	}

	Camera::Camera() {

	}

	Camera::~Camera() {

	}

	void Camera::ApplyEffects() {
		if (m_shakeTime > 0.0f) {
			m_shakeTime -= dt;

			if (m_shakeTime <= 0.0f)
				m_view.setCenter(m_sc.Center);
			else
				Shake();
		}
	}

	void Camera::Shake() {
		m_view.setCenter(m_sc.Center + m_sc.Pos[m_sc.CurrentPos]);

		if (++m_sc.CurrentPos > 6)
			m_sc.CurrentPos = 0;
	}

	void Camera::Resize(const sf::Vector2f& size) {
		m_view.setSize(size);
		m_view.setCenter(size / 2.0f);
		Projection = Ortho(0.0f, size.x, size.y, 0.0f, -1.0f, 1.0f);
	}

	Camera Camera::s_instance;

	// Initializing shake context
	void Camera::ShakeContext::CreatePos(const int& power) {
		for (int i = 0; i < 6; i++)
			Pos[i] = sf::Vector2f((float)Random::Get().Int(-power, power), (float)Random::Get().Int(-power, power));
	}
}
