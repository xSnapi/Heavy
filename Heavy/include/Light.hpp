#pragma once
#include <SFML/System/Vector2.hpp>

#include "Light World.hpp"

namespace hv {
	struct SpotLight
	{
		SpotLight();

		SpotLight(sf::Vector2f pos);

		SpotLight(float radius);

		SpotLight(sf::Vector2f pos, float radius);

		SpotLight(const SpotLight& other);

		~SpotLight();

		SpotLight* operator=(const SpotLight& other);

		void SetPosition(sf::Vector2f pos);
		void SetRadius(float radius);
		void SetAttenuation(float attenuation);
		void SetPower(float power);

		sf::Vector2f GetPosition() const;
		float GetRadius() const;
		float GetAttenuation() const;
		float GetPower() const;

	private:
		Light* m_light = nullptr;

		void CopyLight(const SpotLight& other);
	};
}