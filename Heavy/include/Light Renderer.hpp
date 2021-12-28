#pragma once
#include <unordered_set>
#include <list>

#include "Heavy Renderer.hpp"

namespace std {
	template<> struct hash<sf::Vector2f> {
		std::size_t operator()(const sf::Vector2f& p) const noexcept {
			return p(p);
		}
	};
}

namespace hv {
	class  LightWorld;
	struct Edge;
	struct Light;

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

namespace hv {
	

	class LightRenderer {
	public:
		LightRenderer();
		~LightRenderer();

		void Resize(sf::Vector2u size);

		void DrawLights(Renderer& renderer);

	private:
		sf::Shader m_spotShader;

		sf::Shader m_mergeShader;

		sf::RenderTexture m_lightMask;
		sf::RenderTexture m_lightMaskBuffer;

		sf::RenderTexture m_shadowMask;
		sf::RenderTexture m_shadowMaskBuffer;

		sf::Sprite m_texturer;

		std::vector<Point>				 m_RayPoints;
		std::vector<sf::Vertex>			 m_ShadowVertexArray;
		std::unordered_set<sf::Vector2f> m_Vertices;

		std::list<Edge*> m_ShadowBox;

		void CreateLightMask();
		void CreateShadowMask(const Light& light);

		void CastRays(sf::Vector2f pos);

		void LoadShaders();

		void ResizeTextures(sf::Vector2u size);
		void ResizeShaders(sf::Vector2f size);
		void ResizeShadowBox();

		friend class LightWorld;
	};
}