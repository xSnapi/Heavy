#pragma once
#include <unordered_set>
#include <string>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "Heavy Renderer.hpp"

namespace hv {
	enum class LightType { Point = 0, Spot };

	struct Light
	{
		sf::Vector2f Position;
		float		 Radius = 0.0f;
		LightType	 Type;

		Light(LightType type) :
			Type(type)
		{

		}
	};
}

namespace hv {
	class  Runtime;
	struct PointLight;
	struct SpotLight;

	class LightWorld {
	public:
		static LightWorld& Get();

		bool  Enabled	 = false;

		void SetLightLevel(float level);
		float GetLightLevel() const;

		void RenderLights(sf::RenderWindow& window, const sf::Texture& frame);
	private:
		LightWorld();
		~LightWorld();

		sf::RenderTexture*	m_lightMask	  = nullptr;
		sf::Shader*			m_MaskShader  = nullptr;
		sf::Shader*			m_SpotShader  = nullptr;
		sf::Sprite			m_frameSprite;

		std::unordered_set<Light*>  m_Lights;

		float m_lightLevel = 1.0f;

		void Init(sf::Vector2u size);

		void Resize(sf::Vector2u size);

		Light* CreateLight(LightType type);
		void DestroyLight(Light* light);

		struct ShaderSource
		{
			const static std::string MaskVertex;
			const static std::string MaskFragment;

			const static std::string SpotVertex;
			const static std::string SpotFragment;
		};

		friend class  Runtime;
		friend struct PointLight;
		friend struct SpotLight;

		static LightWorld s_instance;
	};
}