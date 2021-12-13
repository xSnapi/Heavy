#pragma once

/*
	Heavy Utilities.hpp
	Class with conversions or functions that dosen't fit anywhere else,
	but are used pretty often
*/

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector3.hpp>

namespace hv {
	inline sf::Color Vec3ToColor(const sf::Vector3f& v, sf::Uint8 alpha = 255) {
		return sf::Color((sf::Uint8)v.x, (sf::Uint8)v.y, (sf::Uint8)v.z, alpha);
	}

	inline sf::Vector3f ColorToVec3(const sf::Color& c) { // Note alpha is striped here
		return sf::Vector3f(c.r, c.g, c.b);
	}

	inline sf::Vector3f ColorToNormalizedVec3(const sf::Color& c) { // Note alpha is striped here
		return sf::Vector3f((float)c.r / 255.0f, (float)c.g / 255.0f, (float)c.b / 255.0f);
	}
}