#include <hvpch.h>
#include "Light.hpp"

namespace hv {
	SpotLight::SpotLight() {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);
	}

	SpotLight::SpotLight(sf::Vector2f pos) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		m_light->Position = pos;

		LightWorld::Get().m_changed = true;
	}

	SpotLight::SpotLight(float radius) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		m_light->Radius = radius;

		LightWorld::Get().m_changed = true;
	}

	SpotLight::SpotLight(sf::Vector2f pos, float radius) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		m_light->Position = pos;
		m_light->Radius   = radius;

		LightWorld::Get().m_changed = true;
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

		LightWorld::Get().m_changed = true;
	}

	void SpotLight::SetRadius(float radius) {
		m_light->Radius = radius;

		LightWorld::Get().m_changed = true;
	}

	void SpotLight::SetAttenuation(float attenuation) {
		m_light->Attenuation = attenuation;

		LightWorld::Get().m_changed = true;
	}

	void SpotLight::SetPower(float power) {
		m_light->LightPower = power;

		LightWorld::Get().m_changed = true;
	}

	void SpotLight::SetDrawable(bool drawable) {
		m_light->Drawable = drawable;

		LightWorld::Get().m_changed = true;
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

	bool SpotLight::GetDrawable() const {
		return m_light->Drawable;
	}

	void SpotLight::CopyLight(const SpotLight& other) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		*m_light = *other.m_light;

		LightWorld::Get().m_changed = true;
	}
}