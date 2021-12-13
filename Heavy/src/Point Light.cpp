#include <hvpch.h>
#include "Point Light.hpp"

#include "Heavy Utilities.hpp"

namespace hv {	
	PointLight::PointLight() {
		m_Positions.resize(MAX_LIGHTS);

		m_Colors.resize(MAX_LIGHTS);

		m_Radii.resize(MAX_LIGHTS);

		m_lightShader.loadFromMemory(ShaderSource::Vertex, ShaderSource::Fragment);

		SetLightIntensity(0.03f);
	}

	PointLight::~PointLight() {

	}

	sf::Shader* PointLight::GetShader() {
		if (m_size) {
			m_lightShader.setUniformArray("u_positions", m_Positions.data(), m_size);
			m_lightShader.setUniformArray("u_colors", m_Colors.data(), m_size);
			m_lightShader.setUniformArray("u_radii", m_Radii.data(), m_size);
		}
		else {
			m_lightShader.setUniform("u_positions[0]", sf::Vector2f());
			m_lightShader.setUniform("u_colors[0]", sf::Vector3f());
			m_lightShader.setUniform("u_radii[0]", 0.0f);
		}

		m_Positions.clear();
		m_Positions.resize(MAX_LIGHTS);

		m_Colors.clear();
		m_Colors.resize(MAX_LIGHTS);

		m_Radii.clear();
		m_Radii.resize(MAX_LIGHTS);

		m_size = 0;

		return &m_lightShader;
	}

	void PointLight::AddLightSource(LightSource source) const {
		HV_DEBUG_ASSERT(m_size < MAX_LIGHTS);

		m_Positions[m_size] = source.Position;
		m_Radii[m_size]		= source.Radius;
		m_Colors[m_size]	= ColorToNormalizedVec3(source.Color);

		m_size++;
		m_lightShader.setUniform("u_size", m_size);
	}


	void PointLight::SetLightIntensity(float intensity) {
		m_lightIntensity = intensity;

		m_lightShader.setUniform("u_lightIntensity", m_lightIntensity);
	}

	void PointLight::SetResolution(sf::Vector2f res) {
		m_lightShader.setUniform("u_resolution", res);
	}

	LightSource::LightSource() {

	}

	LightSource::LightSource
	(
		sf::Vector2f position, 
		float radius
	) : 
		Position(position), 
		Radius(radius)
	{
		
	}

	LightSource::~LightSource() {
		
	}

}

namespace hv {
	const std::string PointLight::ShaderSource::Vertex = R"(
		varying vec2 st;
		
		void main() {
		    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		
		    st = gl_TextureMatrix[0] * gl_MultiTexCoord0;
		
		    gl_FrontColor = gl_Color;
		}
	)";

	const std::string PointLight::ShaderSource::Fragment = R"(
		varying vec2 st;

		uniform sampler2D texture;
		
		uniform vec2  u_resolution;
		uniform vec2  u_positions[99];
		uniform vec3  u_colors[99];
		uniform float u_radii[99];
		uniform float u_lightIntensity;
		uniform int   u_size;
		
		vec3 Circle(vec2 position, vec3 color, float radius, vec3 sub) {
			float circleShape = sqrt(pow(position.x, 2.0) + pow(position.y, 2.0));
			float circle = smoothstep(radius, radius + 0.15, 1.0 - circleShape);
			
			// Uncomment line here to stop light from merging
			//return (color - vec3(u_lightIntensity)) * circle;
			return (color - sub) * circle;
		}
		
		void main() {
			vec2 uv = gl_FragCoord.xy / u_resolution;
			
			float aspect = u_resolution.x / u_resolution.y;
			
			uv.y = 1.0 - uv.y;	
	
			uv.x *= aspect;		

			vec4 pixel = texture2D(texture, st.xy);
		
			vec3 canvas = vec3(u_lightIntensity);
		
			for (int i = 0; i < u_size; i++) {
				vec3 circle = Circle(uv - (u_positions[i] / u_resolution), u_colors[i], 1.0 - (u_radii[i] / u_resolution.x), canvas);
		
				canvas += circle;
			}
				
			gl_FragColor = pixel * vec4(canvas, 1.0);
		};
	)";
}
