#include <hvpch.h>
#include "Light.hpp"

namespace hv {
	SpotLight::SpotLight() {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);
	}

	SpotLight::SpotLight(sf::Vector2f pos) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		m_light->Position = pos;
	}

	SpotLight::SpotLight(float radius) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		m_light->Radius = radius;
	}

	SpotLight::SpotLight(sf::Vector2f pos, float radius) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		m_light->Position = pos;
		m_light->Radius   = radius;
	}

	SpotLight::SpotLight(const SpotLight& other) {
		CopyLight(other);
	}

	SpotLight::~SpotLight() {
		LightWorld::Get().DestroyLight(m_light);
	}

	SpotLight* SpotLight::operator=(const SpotLight& other) {
		CopyLight(other);

		return this;
	}

	void SpotLight::SetPosition(sf::Vector2f pos) {
		m_light->Position = pos;
	}

	void SpotLight::SetRadius(float radius) {
		m_light->Radius = radius;
	}

	void SpotLight::SetAttenuation(float attenuation) {
		m_light->Attenuation = attenuation;
	}

	void SpotLight::SetPower(float power) {
		m_light->LightPower = power;
	}

	sf::Vector2f SpotLight::GetPosition() const {
		return m_light->Position;
	}

	float SpotLight::GetRadius() const {
		return m_light->Radius;
	}

	float SpotLight::GetAttenuation() const {
		return m_light->Attenuation;
	}

	float SpotLight::GetPower() const {
		return m_light->LightPower;
	}

	void SpotLight::CopyLight(const SpotLight& other) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		*m_light = *other.m_light;
	}
}