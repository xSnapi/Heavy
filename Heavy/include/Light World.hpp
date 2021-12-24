#pragma once
#include <unordered_set>
#include <string>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "Heavy Renderer.hpp"

namespace hv {
	enum class LightType { Point = 0, Spot };

	struct Light
	{
		sf::Vector2f Position;

		float		 Radius		 = 0.0f;
		float		 Attenuation = 0.15f;

		float		 LightPower  = 1.0f;

		LightType	 Type;
		sf::Color	 Color = sf::Color::White;

		Light(LightType type) :
			Type(type)
		{

		}
	};
}

namespace hv {
	struct PointLight;
	struct SpotLight;
}

namespace hv {
	class LightWorld {
	public:
		void Init(sf::Vector2u size);

		void Update();

		static LightWorld& Get();

		void Resize(sf::Vector2u size);

		void SetLightLevel(float level);
		void SetLightEnabled(bool enabled);

		float GetLightLevel() const;
		bool LightEnabled() const;

		void RenderLights(Renderer& renderer, const sf::Sprite& frame);
	private:
		LightWorld();
		~LightWorld();

		float m_lightLevel	= 0.0f;
		bool  m_enabled		= false;

		sf::Shader* m_maskShader = nullptr;
		sf::Shader* m_spotShader = nullptr;

		sf::RenderTexture* m_frame	    = nullptr;

		sf::RenderTexture* m_lightMask  = nullptr;
		sf::RenderTexture* m_maskBuffer = nullptr;

		sf::Sprite m_maskSprite;
		sf::Sprite m_frameSprite;

		std::unordered_set<Light*> m_Lights;

		void DestroyLight(Light* light);
		Light* CreateLight(LightType type);

		friend struct PointLight;
		friend struct SpotLight;

		static LightWorld s_instance;

		struct ShaderSource
		{
			const static std::string MaskVertex;
			const static std::string MaskFragment;
			const static std::string SpotVertex;
			const static std::string SpotFragment;
		};
	};
}