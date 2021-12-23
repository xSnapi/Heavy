#include <hvpch.h>
#include "Light.hpp"

namespace hv {
	SpotLight::SpotLight() {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);
	}

	SpotLight::SpotLight(const SpotLight& other) {

	}

	SpotLight::~SpotLight() {
		LightWorld::Get().DestroyLight(m_light);
	}

	SpotLight* SpotLight::operator=(const SpotLight& other) {

		return nullptr;
	}

	void SpotLight::SetPosition(sf::Vector2f pos) {
		m_light->Position = pos;
	}

	void SpotLight::SetRadius(float radius) {
		m_light->Radius = radius;
	}
}