#include <hvpch.h>
#include "Light.hpp"

namespace hv {
	LightEdge::LightEdge() {
		m_edge = LightWorld::Get().CreateEdge();
	}

	LightEdge::LightEdge(sf::Vector2f start, sf::Vector2f end) {
		m_edge = LightWorld::Get().CreateEdge();

		m_edge->Start = start;
		m_edge->End   = end;
	}

	LightEdge::LightEdge(const LightEdge& other) {
		CopyEdge(other);
	}

	LightEdge::~LightEdge() {
		LightWorld::Get().DestroyEdge(m_edge);
	}

	LightEdge* LightEdge::operator=(const LightEdge& other) {
		CopyEdge(other);

		return this;
	}

	void LightEdge::Set(sf::Vector2f start, sf::Vector2f end) {
		m_edge->Start = start;
		m_edge->End   = end;
	}

	void LightEdge::SetStart(sf::Vector2f start) {
		m_edge->Start = start;
	}

	void LightEdge::SetEnd(sf::Vector2f end) {
		m_edge->End = end;
	}

	sf::Vector2f LightEdge::GetStart() const {
		return m_edge->Start;
	}

	sf::Vector2f LightEdge::GetEnd() const {
		return m_edge->End;
	}

	void LightEdge::CopyEdge(const LightEdge& other) {
		m_edge = LightWorld::Get().CreateEdge();

		*m_edge = *other.m_edge;
	}
}

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
		if(pos != m_light->Position)
			LightWorld::Get().m_changed = true;

		m_light->Position = pos;
	}

	void SpotLight::SetRadius(float radius) {
		if (radius != m_light->Radius)
			LightWorld::Get().m_changed = true;

		m_light->Radius = radius;
	}

	void SpotLight::SetAttenuation(float attenuation) {
		if (attenuation != m_light->Attenuation)
			LightWorld::Get().m_changed = true;

		m_light->Attenuation = attenuation;
	}

	void SpotLight::SetPower(float power) {
		if(power != m_light->LightPower)
			LightWorld::Get().m_changed = true;
		
		m_light->LightPower = power;
	}

	void SpotLight::SetDrawable(bool drawable) {
		if(drawable != m_light->Drawable)
			LightWorld::Get().m_changed = true;

		m_light->Drawable = drawable;
	}

	void SpotLight::SetColor(sf::Color color) {
		m_light->Color = color;
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