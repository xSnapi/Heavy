#include <hvpch.h>
#include "Point Light.hpp"

#include "Heavy Utilities.hpp"

namespace hv {
	GlowEffect::GlowEffect() {
		if (!s_shader) {
			//TODO: powiedz ze to potrzebne ten leak
			s_shader = new sf::Shader();
			s_shader->loadFromMemory(ShaderSource::Vertex, ShaderSource::Fragment);
		}

		// Because of triangles
		m_vertices[0].texCoords = sf::Vector2f(0.0f, 0.0f);
		m_vertices[1].texCoords = sf::Vector2f(1.0f, 0.0f);
		m_vertices[2].texCoords = sf::Vector2f(1.0f, 1.0f);

		m_vertices[3].texCoords = sf::Vector2f(0.0f, 0.0f);
		m_vertices[4].texCoords = sf::Vector2f(0.0f, 1.0f);
		m_vertices[5].texCoords = sf::Vector2f(1.0f, 1.0f);
	}

	GlowEffect::~GlowEffect() {

	}

	void GlowEffect::SetRadius(float radius) {
		m_radius = radius;

		TranslateVertices();
	}

	void GlowEffect::SetPosition(sf::Vector2f pos) {
		m_position = pos;

		TranslateVertices();
	}

	void GlowEffect::TranslateVertices() {
		m_vertices[0].position = sf::Vector2f(m_position.x - m_radius, m_position.y - m_radius);
		m_vertices[1].position = sf::Vector2f(m_position.x + m_radius, m_position.y - m_radius);
		m_vertices[2].position = sf::Vector2f(m_position.x + m_radius, m_position.y + m_radius);

		m_vertices[3].position = sf::Vector2f(m_position.x - m_radius, m_position.y - m_radius);
		m_vertices[4].position = sf::Vector2f(m_position.x - m_radius, m_position.y + m_radius);
		m_vertices[5].position = sf::Vector2f(m_position.x + m_radius, m_position.y + m_radius);
	}

	void GlowEffect::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		s_shader->setUniform("u_color", ColorToNormalizedVec4(Color));

		states.shader = s_shader;

		target.draw(m_vertices, 6, sf::Triangles, states);
	}
	
	const std::string GlowEffect::ShaderSource::Vertex   = R"(
		varying vec2 uv;	
		void main()
		{
			uv = gl_TextureMatrix[0] * gl_MultiTexCoord0 * 2.0 - 1.0;
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		};
	)";

	const std::string GlowEffect::ShaderSource::Fragment = R"(
		#version 330 core
		
		const float alphaPower = 2.0;
		
		layout(location = 0) out vec4 color;
		
		uniform vec4 u_color;
		
		in vec2 uv;
		void main()
		{
		    float distance = 1.0 - length(uv);
			
			if(distance < -0.05)
				discard;
		
			color = vec4(u_color.rgb, pow(distance, alphaPower) * u_color.a);
		};
	)";

	sf::Shader* GlowEffect::s_shader = nullptr;
}