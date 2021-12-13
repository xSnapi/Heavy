#pragma once

#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Vector2.hpp>

namespace hv {
	constexpr uint32_t MAX_LIGHTS = 99;

	struct LightSource 
	{
		sf::Vector2f Position;
		sf::Color	 Color  = sf::Color::White;
		float		 Radius = 0.0f;

		LightSource();

		LightSource(sf::Vector2f position, float radius);

		~LightSource();
	};

	class PointLight {
	public:		
		PointLight();
		~PointLight();

		void AddLightSource(LightSource source) const;
		void SetLightIntensity(float intensity);

		void SetResolution(sf::Vector2f res);

		sf::Shader* GetShader();
	private:
		struct ShaderSource
		{
			const static std::string Fragment;
			const static std::string Vertex;
		};

		mutable sf::Shader m_lightShader;

		mutable std::vector<sf::Vector2f> m_Positions;
		mutable std::vector<sf::Vector3f> m_Colors;
		mutable std::vector<float> m_Radii;

		mutable int m_size = 0;

		float m_lightIntensity = 0.5f;
	};
}