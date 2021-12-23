#pragma once
#include <SFML/System/Vector2.hpp>

#include "Light World.hpp"

namespace hv {
	struct SpotLight
	{
		SpotLight();

		SpotLight(const SpotLight& other);

		~SpotLight();

		SpotLight* operator=(const SpotLight& other);

		void SetPosition(sf::Vector2f pos);
		void SetRadius(float radius);
	private:
		Light* m_light = nullptr;
	};
}