#pragma once
#include <unordered_set>
#include <list>
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

	struct Point 
	{
		Point(sf::Vector2f position, float angle) :
			Position(position),
			Angle(angle)
		{

		}

		sf::Vector2f Position;
		float		 Angle = 0.0f;
	};
}

namespace std {
	template<> struct hash<sf::Vector2f> {
		std::size_t operator()(const sf::Vector2f& p) const noexcept {
			return p(p);
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

		// Modifiers
		void SetShadowSoftness(float softness);
		void SetShadowEnabled(bool enabled);
		void SmoothenShadow(bool smoothen);

		void SetLightLevel(float level);
		void SetLightEnabled(bool enabled);

		// Accessors
		float GetShadowSoftness() const;
		bool  ShadowsEnabled() const;

		float GetLightLevel() const;
		bool  LightEnabled() const;

		void RenderLights(Renderer& renderer);
	private:
		LightWorld();
		~LightWorld();

		float m_shadowSoftness = 0.5f;
		float m_lightLevel	   = 0.0f;

		bool m_shadowsEnabled = false;
		bool m_lightEnabled   = false;

		sf::Shader* m_maskShader = nullptr;
		sf::Shader* m_spotShader = nullptr;

		sf::Shader* m_blurShader   = nullptr;
		sf::Shader* m_shadowShader = nullptr;

		sf::Shader* m_mergeShader = nullptr;
		sf::Shader* m_lightMergeShader = nullptr;

		sf::RenderTexture* m_lightMask  = nullptr;
		sf::RenderTexture* m_maskBuffer = nullptr;

		sf::RenderTexture* m_shadowBuffer = nullptr;
		sf::RenderTexture* m_shadowMask   = nullptr;

		sf::Sprite m_maskSprite;
		sf::Sprite m_frameSprite;

		sf::Sprite m_shadowSprite;
		sf::Sprite m_shadowMaskSprite;

		std::unordered_set<Light*> m_Lights;
		std::unordered_set<Edge*>  m_Edges;

		std::vector<Point> m_RayPoints;
		std::unordered_set<sf::Vector2f> m_Vertices;
		std::vector<sf::Vertex> m_ShadowVertexArray;

		std::list<Edge*> m_OldShadowBox;

		bool m_changed = false;

		void DestroyLight(Light* light);
		Light* CreateLight(LightType type);

		void DestroyEdge(Edge* edge);
		Edge* CreateEdge(sf::Vector2f start = { }, sf::Vector2f end = { });

		void CreateShadowMask(sf::Vector2f pos);

		friend struct PointLight;
		friend struct SpotLight;
		friend struct LightEdge;

		static LightWorld s_instance;

		struct ShaderSource
		{
			const static std::string MaskVertex;
			const static std::string MaskFragment;

			const static std::string SpotVertex;
			const static std::string SpotFragment;

			const static std::string BlurVertex;
			const static std::string BlurFragment;

			const static std::string ShadowVertex;
			const static std::string ShadowFragment;

			const static std::string ShadowMergeVertex;
			const static std::string ShadowMergeFragment;

			const static std::string LightShadowMergeVertex;
			const static std::string LightShadowMergeFragment;
		};
	};
}
