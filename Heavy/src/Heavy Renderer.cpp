#include <hvpch.h>
#include "Heavy Renderer.hpp"

#include "Heavy Debug.hpp"

namespace hv {
	Renderer::Renderer() {

	}

	Renderer::~Renderer() {

	}

	void Renderer::draw(const std::string& str, const sf::Vector2f& position, const sf::Color& color) {
		if (!m_font) {
			Debug::Log(Color::Red, "[Couldn't render text font was't initialized properly]\n");
			return;
		}

		m_text.setFont(*m_font);
		m_text.setString(str);
		m_text.setPosition(position);
		m_text.setFillColor(color);

		m_frame::draw(m_text);
	}

	void Renderer::LoadFont(const std::string& path) {
		delete m_font;

		m_font = new sf::Font();
		m_font->loadFromFile(path);
	}

	void Renderer::Resize(sf::Vector2u size) {
		m_frame::create(size.x, size.y);
		m_frame::clear();
		m_frame::display();
	}

	void Renderer::clear() {
		m_frame::clear(ClearColor);
	}

	sf::RenderWindow& Renderer::GetWindow() const {
		return *m_window;
	}

	void Renderer::SetWindow(sf::RenderWindow& window) {
		m_window = &window;
	}

	const sf::Sprite& Renderer::GetFrame() {
		m_frameSprite.setTexture(m_frame::getTexture());
		return m_frameSprite;
	}

	void Renderer::DrawCircle(sf::RenderTarget& target, sf::Vector2f position, float radius, sf::Color color, bool diagonalBar) {
		if (radius <= 0.0f)
			return;

		const uint32_t points = 32;
		float angle = 360.0f / (float)points;

		sf::Vector2f pos = position;

		pos.y += radius; // starting point of circle

		sf::Vertex vertices[points + 1];

		sf::Transform t;
		t.rotate(angle, position);

		// We transform point around circle position for angle based on number of points
		for (uint32_t i = 0; i <= points; i++) {
			vertices[i] = sf::Vertex(pos, color);
			pos = t.transformPoint(pos);
		}

		pos = position;
		pos.y -= radius;

		if (diagonalBar) {
			sf::Vertex line[2]; // We add line that goes throught middle of circle

			t.rotate(-(angle - 45.0f), position);

			line[0] = sf::Vertex(t.transformPoint(pos), color);

			t.rotate(180.0f, position);

			line[1] = sf::Vertex(t.transformPoint(pos), color);
			
			target.draw(line, 2, sf::Lines);
		}

		target.draw(vertices, points + 1, sf::LineStrip);
	}
}