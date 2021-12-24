#include <hvpch.h>
#include "Heavy Renderer.hpp"

namespace hv {
	Renderer::Renderer() {

	}

	Renderer::~Renderer() {

	}

	void Renderer::Resize(sf::Vector2u size) {
		m_frame::create(size.x, size.y);
		m_frame::clear();
	}

	void Renderer::clear() {
		m_frame::clear(ClearColor);
	}

	const sf::Sprite& Renderer::GetFrame() {
		m_frameSprite.setTexture(m_frame::getTexture());
		return m_frameSprite;
	}
}