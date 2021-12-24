#include <hvpch.h>
#include "Light World.hpp"

#include "Camera.hpp"
#include "Heavy Utilities.hpp"

namespace hv {
	LightWorld LightWorld::s_instance;

	LightWorld::LightWorld() {

	}

	LightWorld::~LightWorld() {

	}

	void LightWorld::Init(sf::Vector2u size) {
		{
			m_frame		 = new sf::RenderTexture();
			m_lightMask  = new sf::RenderTexture();
			m_maskBuffer = new sf::RenderTexture();
		}

		{
			m_maskShader = new sf::Shader();
			m_maskShader->loadFromMemory(ShaderSource::MaskVertex, ShaderSource::MaskFragment);
			SetLightLevel(m_lightLevel);
		}

		{
			m_spotShader = new sf::Shader();
			m_spotShader->loadFromMemory(ShaderSource::SpotVertex, ShaderSource::SpotFragment);
		}

		Resize(size);
	}

	void LightWorld::Update() {

	}

	LightWorld& LightWorld::Get() {
		return s_instance;
	}

	void LightWorld::Resize(sf::Vector2u size) {
		if (m_lightMask && m_frame) {
			m_lightMask->create(size.x, size.y);
			m_frame->create(size.x, size.y);

			m_maskBuffer->create(size.x, size.y);
		}

		if (m_spotShader)
			m_spotShader->setUniform("u_resolution", (sf::Vector2f)size);
	}

	void LightWorld::SetLightLevel(float level) {
		m_lightLevel = level;

		if (m_maskShader)
			m_maskShader->setUniform("u_level", m_lightLevel);
	}

	void LightWorld::SetLightEnabled(bool enabled) {
		m_enabled = enabled;
	}

	float LightWorld::GetLightLevel() const {
		return m_lightLevel;
	}

	bool LightWorld::LightEnabled() const {
		return m_enabled;
	}

	void LightWorld::RenderLights(Renderer& renderer, const sf::Sprite& frame) {
		m_lightMask->clear();
		m_maskBuffer->clear(sf::Color::White);

		//TODO: zoptymalizowaæ u¿ywaj¹c algorytmu sprawdzaj¹cego czy cia³o jest w poprawnym miejscu
		for (auto& l : m_Lights) {
			m_spotShader->setUniform("u_attenuation", l->Attenuation <= 0.0f ? 0.0005f : l->Attenuation);
			m_spotShader->setUniform("u_lightPower", l->LightPower);
			m_spotShader->setUniform("u_position", l->Position);
			m_spotShader->setUniform("u_radius", l->Radius);
			m_spotShader->setUniform("u_color", sf::Vector3f(ColorToNormalizedVec3(l->Color)));

			m_maskSprite.setTexture(m_maskBuffer->getTexture());
			m_lightMask->draw(m_maskSprite, m_spotShader);

			m_maskSprite.setTexture(m_lightMask->getTexture());

			m_maskBuffer->draw(m_maskSprite);
			m_maskBuffer->display();
		}
		
		m_lightMask->display();

		m_maskShader->setUniform("u_light", m_maskBuffer->getTexture());

		//renderer.draw(m_maskSprite);
		renderer.draw(frame, m_maskShader);
	}

	void LightWorld::DestroyLight(Light* light) {
		m_Lights.erase(light);

		delete light;
	}

	Light* LightWorld::CreateLight(LightType type) {
		Light* buffer = new Light(type);

		m_Lights.emplace(buffer);

		return buffer;
	}

	const std::string LightWorld::ShaderSource::MaskVertex	 = R"(
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

	const std::string LightWorld::ShaderSource::SpotVertex   = R"(
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

uniform vec2  u_resolution;
uniform vec2  u_position;
uniform vec3  u_color;
uniform float u_radius;
uniform float u_attenuation;
uniform float u_lightPower;

vec3 Circle(vec2 position, vec3 color, float radius) {
	float circleShape = length(position);
	float circle = smoothstep(radius, radius + u_attenuation, 1.0 - circleShape);
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
	o_color = pixel * vec4(1.0 - Circle(uv - pos, u_color, rad) * u_lightPower, 1.0);
}
)";

}