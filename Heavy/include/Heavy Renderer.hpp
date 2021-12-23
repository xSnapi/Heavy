#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace hv {
	class Renderer : private sf::RenderTexture {
	public:
		using sf::RenderTexture::draw;
		using sf::RenderTexture::clear;
		using sf::RenderTexture::display;

		Renderer();
		~Renderer();

		void Resize(sf::Vector2u size);
		
		const sf::Texture& GetFrame() const;

	private:
		typedef sf::RenderTexture m_frame;
	};
}