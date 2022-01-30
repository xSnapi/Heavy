#include <hvpch.h>
#include "Light World.hpp"

#include "Camera.hpp"
#include "Heavy Utilities.hpp"

#include "Input.hpp"

using namespace hv;

LightWorld LightWorld::s_instance;

LightWorld::LightWorld() {
	m_Lights.reserve(2048);

	m_Edges.reserve(2048);
}

LightWorld::~LightWorld() {

}

void LightWorld::Init(LightRenderer& lightRenderer) {
	m_lightRenderer = &lightRenderer;

	m_lightRenderer->m_mergeShader.setUniform("u_level", m_lightLevel < 0.0f ? 0.0f : m_lightLevel);
}

LightWorld& LightWorld::Get() {
	return s_instance;
}

void LightWorld::SetShadowSoftness(float softness) {
	m_shadowSoftness = softness;
}

void LightWorld::SetShadowEnabled(bool enabled) {
	m_shadowsEnabled = enabled;
}

void LightWorld::SetLightLevel(float level) {
	m_lightLevel = level;

	if (m_lightRenderer)
		m_lightRenderer->m_mergeShader.setUniform("u_level", m_lightLevel < 0.0f ? 0.0f : m_lightLevel);
}

void LightWorld::SetLightEnabled(bool enabled) {
	m_lightEnabled = enabled;
}

void LightWorld::DisableDebugDraw() {
	m_debugDraw = false;
}

float LightWorld::GetShadowSoftness() const {
	return m_shadowSoftness;
}

bool LightWorld::ShadowsEnabled() const {
	return m_shadowsEnabled;
}

float LightWorld::GetLightLevel() const {
	return m_lightLevel;
}

bool LightWorld::LightEnabled() const {
	return m_lightEnabled;
}

void LightWorld::DestroyLight(Light* light) {
	m_Lights.erase(light);

	delete light;
}

Light* LightWorld::CreateLight(LightType type) {
	Light* buffer = new Light(type);

	m_Lights.emplace(buffer);

	return buffer;
}

void LightWorld::DestroyEdge(Edge* edge) {
	m_Edges.erase(edge);

	delete edge;
}

Edge* LightWorld::CreateEdge(sf::Vector2f start, sf::Vector2f end) {
	Edge* buffer = new Edge({ start, end });

	m_Edges.emplace(buffer);

	return buffer;
}

