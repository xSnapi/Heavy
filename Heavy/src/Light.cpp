#include <hvpch.h>
#include "Light.hpp"

#include "Heavy ImGui.hpp"

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
	PointLight::PointLight() {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);
	}

	PointLight::PointLight(sf::Vector2f pos) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		m_light->Position = pos;

		LightWorld::Get().m_changed = true;
	}

	PointLight::PointLight(float radius) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		m_light->Radius = radius;

		LightWorld::Get().m_changed = true;
	}

	PointLight::PointLight(sf::Vector2f pos, float radius) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		m_light->Position = pos;
		m_light->Radius   = radius;

		LightWorld::Get().m_changed = true;
	}

	PointLight::PointLight(const PointLight& other) {
		CopyLight(other);
	}

	PointLight::~PointLight() {
		LightWorld::Get().DestroyLight(m_light);
	}

	PointLight* PointLight::operator=(const PointLight& other) {
		CopyLight(other);

		return this;
	}

	void PointLight::SetPosition(sf::Vector2f pos) {
		if(pos != m_light->Position)
			LightWorld::Get().m_changed = true;

		m_light->Position = pos;
	}

	void PointLight::SetRadius(float radius) {
		if (radius != m_light->Radius)
			LightWorld::Get().m_changed = true;

		m_light->Radius = radius;
	}

	void PointLight::SetAttenuation(float attenuation) {
		if (attenuation != m_light->Attenuation)
			LightWorld::Get().m_changed = true;

		m_light->Attenuation = attenuation;
	}

	void PointLight::SetPower(float power) {
		if(power != m_light->LightPower)
			LightWorld::Get().m_changed = true;
		
		m_light->LightPower = power;
	}

	void PointLight::SetDrawable(bool drawable) {
		if(drawable != m_light->Drawable)
			LightWorld::Get().m_changed = true;

		m_light->Drawable = drawable;
	}

	void PointLight::SetColor(sf::Color color) {
		m_light->Color = color;
	}

	sf::Vector2f PointLight::GetPosition() const {
		return m_light->Position;
	}

	float PointLight::GetRadius() const {
		return m_light->Radius;
	}

	float PointLight::GetAttenuation() const {
		return m_light->Attenuation;
	}

	float PointLight::GetPower() const {
		return m_light->LightPower;
	}

	bool PointLight::GetDrawable() const {
		return m_light->Drawable;
	}

	void PointLight::DisplayImGuiInfo(const char* tabName) const {
		if (ImGui::CollapsingHeader(tabName)) {
			auto& changed = LightWorld::Get().m_changed;

			float* pos[2]
			{
				&m_light->Position.x,
				&m_light->Position.y,
			};

			if (ImGui::DragFloat2("Position", *pos, 0.5f))
				changed = true;
			if (ImGui::DragFloat("Radius", &m_light->Radius, 0.5f))
				changed = true;
			if (ImGui::DragFloat("Attenuation", &m_light->Attenuation, 0.001f, 0.0f))
				changed = true;
			if (ImGui::DragFloat("Light Power", &m_light->LightPower, 0.5f))
				changed = true;
			
			float color[3]
			{
				(float)m_light->Color.r / 255.0f,
				(float)m_light->Color.g / 255.0f,
				(float)m_light->Color.b / 255.0f,
			};

			if (ImGui::ColorPicker3(tabName, color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_RGB)) {
				m_light->Color = sf::Color(uint8_t(color[0] * 255.0f), uint8_t(color[1] * 255.0f), uint8_t(color[2] * 255.0f), 255);
				changed = true;
			}
		}
	}

	void PointLight::CopyLight(const PointLight& other) {
		m_light = LightWorld::Get().CreateLight(LightType::Spot);

		*m_light = *other.m_light;

		LightWorld::Get().m_changed = true;
	}
}