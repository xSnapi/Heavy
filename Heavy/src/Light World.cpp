#include <hvpch.h>
#include "Light World.hpp"

namespace hv {
	LightWorld LightWorld::s_instance;

	LightWorld& LightWorld::Get() { return s_instance; }

	void LightWorld::SetLightLevel(float level) {
		m_lightLevel = level;

		if (m_MaskShader)
			m_MaskShader->setUniform("u_level", m_lightLevel);
	}

	float LightWorld::GetLightLevel() const {
		return m_lightLevel;
	}

	LightWorld::LightWorld() {
		
	}

	LightWorld::~LightWorld() {
		for (auto& l : m_Lights)
			delete l;
	}

	static sf::RenderTexture* rx;
	
	void LightWorld::Init(sf::Vector2u size) {
		//TODO: napisz ze ten memory leak moze tu byc
		{	
			m_MaskShader = new sf::Shader();

			m_MaskShader->loadFromMemory(ShaderSource::MaskVertex, ShaderSource::MaskFragment);
			m_MaskShader->setUniform("u_level", m_lightLevel);
		}

		{
			m_SpotShader = new sf::Shader();

			m_SpotShader->loadFromMemory(ShaderSource::SpotVertex, ShaderSource::SpotFragment);
		}

		{
			m_lightMask = new sf::RenderTexture();
			Resize(size);
		}
		rx = new sf::RenderTexture;
		rx->create(800, 800);
	}

	void LightWorld::Resize(sf::Vector2u size) {
		m_lightMask->create(size.x, size.y);

		m_lightMask->clear();

		if (m_SpotShader) {
			m_SpotShader->setUniform("u_resolution", (sf::Vector2f)size);
		}
	}

	void LightWorld::RenderLights(sf::RenderWindow& window, const sf::Texture& frame) {
		// creating light mask

		m_lightMask->clear();
		rx->clear(sf::Color::White);

		for (auto& l : m_Lights) {
			m_SpotShader->setUniform("u_position", l->Position);
			m_SpotShader->setUniform("u_radius", l->Radius);
			m_SpotShader->setUniform("u_color", sf::Glsl::Vec3(1.0, 1.0, 1.0));

			m_lightMask->draw(sf::Sprite(rx->getTexture()), m_SpotShader);

			rx->draw(sf::Sprite(m_lightMask->getTexture()));
			rx->display();
		}

		m_lightMask->display();

		m_MaskShader->setUniform("u_light", rx->getTexture());

		window.draw(sf::Sprite(frame), m_MaskShader);
	}

	void LightWorld::DestroyLight(Light* light) {
		m_Lights.erase(light);

		delete light;
	}

	Light* LightWorld::CreateLight(LightType type) {
		Light* buffer = new Light(type);

		m_Lights.insert(buffer);

		return buffer;
	}

	const std::string LightWorld::ShaderSource::MaskVertex = R"(
		varying vec2 i_tex;

		void main()
		{
			i_tex		= gl_TextureMatrix[0] * gl_MultiTexCoord0;
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		};
	)";

	const std::string LightWorld::ShaderSource::MaskFragment = R"(
		#version 330 core

		precision mediump float;

		layout(location = 0) out vec4 o_color;

		in vec2 i_tex;

		uniform sampler2D texture;
		uniform sampler2D u_light;

		uniform float u_level;
		
		void main() {
			vec4 pixel = texture2D(texture, i_tex);
		
			vec4 lightPixel = texture2D(u_light, i_tex);
			
			vec4 light = vec4(1.0 - lightPixel.x , 1.0 - lightPixel.y, 1.0 - lightPixel.z, 1.0);		
			
			light += u_level;
			
			light = clamp(light, 0.0, 1.0);
			
			o_color = pixel * light;
		}
	)";

	const std::string LightWorld::ShaderSource::SpotVertex = R"(
		varying vec2 i_tex;
		
		void main()
		{
			i_tex		= gl_TextureMatrix[0] * gl_MultiTexCoord0;
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		}
	)";

	const std::string LightWorld::ShaderSource::SpotFragment = R"(
		#version 330
		
		precision mediump float;
		
		layout(location = 0) out vec4 o_color;
		
		in vec2 i_tex;
			
		uniform sampler2D texture;
		
		uniform vec2 u_resolution;
		uniform vec2  u_position;
		uniform vec3  u_color;
		uniform float u_radius;
		
		vec3 Circle(vec2 position, vec3 color, float radius) {
			float circleShape = length(position);
			float circle = smoothstep(radius, radius + 0.15, 1.0 - circleShape);

			return color * circle;
		}

		void main() {
			vec2 uv = gl_FragCoord.xy / u_resolution.xy;
			float aspect = u_resolution.x / u_resolution.y;

			uv.x *= aspect;
			uv.y = 1.0 - uv.y;
	
			vec2  pos = u_position / u_resolution;
			float rad = u_radius   / u_resolution.x;
			rad = 1.0 - rad;		
			
			vec4 pixel = texture2D(texture, i_tex);

			o_color = pixel * vec4(1.0 - Circle(uv - pos, u_color, rad), 1.0);
		}
	)";
}