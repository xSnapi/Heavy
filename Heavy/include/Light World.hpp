#pragma once
#include <unordered_set>
#include <list>
#include <string>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "Light Renderer.hpp"

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

		bool Drawable = true;

		Light(LightType type) :
			Type(type)
		{

		}
	};

	struct Edge
	{
		sf::Vector2f Start;
		sf::Vector2f End;
	};
}

namespace hv {
	struct PointLight;
	struct SpotLight;
}

namespace hv {
	class LightWorld {
	public:
		void Init(LightRenderer& lightRenderer);

		static LightWorld& Get();

		// Modifiers
		void SetShadowSoftness(float softness);
		void SetShadowEnabled(bool enabled);

		void SetLightLevel(float level);
		void SetLightEnabled(bool enabled);

		// Accessors
		float GetShadowSoftness() const;
		bool  ShadowsEnabled() const;

		float GetLightLevel() const;
		bool  LightEnabled() const;
	private:
		LightWorld();
		~LightWorld();

		LightRenderer* m_lightRenderer = nullptr;

		float m_shadowSoftness = 0.5f;
		float m_lightLevel	   = 0.0f;

		bool m_shadowsEnabled = false;
		bool m_lightEnabled   = false;

		bool m_changed = false;

		std::unordered_set<Light*> m_Lights;
		std::unordered_set<Edge*>  m_Edges;

		void DestroyLight(Light* light);
		Light* CreateLight(LightType type);

		void DestroyEdge(Edge* edge);
		Edge* CreateEdge(sf::Vector2f start = { }, sf::Vector2f end = { });

		friend struct PointLight;
		friend struct SpotLight;
		friend struct LightEdge;

		friend class LightRenderer;

		static LightWorld s_instance;
	};
}
