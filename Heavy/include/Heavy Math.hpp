#pragma once

/*
	Heavy Math.hpp
	Class with simple math functions
*/

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

// Math in here dosen't seem heavy, but the engine is ¯\_(ツ)_/¯

namespace hv {
	inline float Lerp(const float& x, const float& y, const float& t) { // Linear interpolation
		return x * (1.0f - t) + y * t;
	}

	inline sf::Vector3f Lerp(const sf::Vector3f& x, const sf::Vector3f& y, const float& t) {
		return x * (1.0f - t) + y * t;
	}

	inline sf::Vector2f Lerp(const sf::Vector2f& x, const sf::Vector2f& y, const float& t) {
		return x * (1.0f - t) + y * t;
	}

	inline float Distance(const sf::Vector2f& v0, const sf::Vector2f& v1) { // Distance between two points
		if (v0.x == v1.x)
			return fabs(v0.y - v1.y);
		else if (v0.y == v1.y)
			return fabs(v0.x - v1.x);

		return sqrtf((v0 - v1).x * (v0 - v1).x + (v0 - v1).y * (v0 - v1).y);
	}

	inline float Sign(const float& var) {
		return var / abs(var);
	}

	inline sf::Vector2f Abs(sf::Vector2f v) {
		v.x = (v.x < 0.0f ? -v.x : v.x);
		v.y = (v.y < 0.0f ? -v.y : v.y);

		return v;
	}

	inline float CrossProduct(const sf::Vector2f& v0, const sf::Vector2f& v1) {
		return v0.x * v1.y - v0.y * v1.x;
	}
}