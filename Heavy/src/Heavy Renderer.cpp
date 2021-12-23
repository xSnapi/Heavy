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

	const sf::Texture& Renderer::GetFrame() const {
		return m_frame::getTexture();
	}
}