#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <string>

namespace hv {
	class GlowEffect : public sf::Drawable {
	public:
		GlowEffect();
		~GlowEffect();

		void SetRadius(float radius);
		void SetPosition(sf::Vector2f pos);

		sf::Color Color = sf::Color(255, 255, 255);
	private:
		sf::Vertex m_vertices[6];

		static sf::Shader*  s_shader;
		sf::Vector2f		m_position;
		float				m_radius = 0.0f;

		struct ShaderSource
		{
			static const std::string Fragment;
			static const std::string Vertex;
		};

		void TranslateVertices();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}