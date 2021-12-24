#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace hv {
	class Renderer : private sf::RenderTexture {
	public:
		using sf::RenderTexture::draw;
		using sf::RenderTexture::display;

		sf::Color ClearColor;

		Renderer();
		~Renderer();

		void Resize(sf::Vector2u size);
		
		void clear();

		const sf::Sprite& GetFrame();

	private:
		typedef sf::RenderTexture m_frame;

		sf::Sprite m_frameSprite;
	};
}