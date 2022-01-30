#include <hvpch.h>
#include "Light Renderer.hpp"

#include "Light World.hpp"
#include "Heavy Utilities.hpp"
#include "Heavy Math.hpp"
#include "Camera.hpp"

using namespace hv;

//TODO: delete
#include "Input.hpp"

static sf::Color EDGE_COLOR = sf::Color(255, 0, 200);

LightRenderer::LightRenderer() {
	/*LoadShaders();
	InitShadowBox();*/
}

LightRenderer::~LightRenderer() {

}

void LightRenderer::Resize(sf::Vector2u size)  {
	ResizeTextures(size);
	ResizeShaders((sf::Vector2f)size);
	ResizeShadowBox();
}

void LightRenderer::DrawLights(Renderer& renderer) {
	CreateLightMask();

	//TODO: delete
	if (!hv::Input::Keyboard::KeyCheck(sf::Keyboard::Space, true)) {
		m_mergeShader.setUniform("u_mergeTx", m_lightMask.getTexture());
		renderer.draw(renderer.GetFrame(), &m_mergeShader);
	}
	else
		renderer.draw(sf::Sprite(m_lightMask.getTexture()));

	if (LightWorld::Get().m_debugDraw)
		DebugDraw(renderer);
}

void LightRenderer::CreateLightMask() {
	auto& changed = LightWorld::Get().m_changed;
	auto& shadows = LightWorld::Get().m_shadowsEnabled;

	if (!changed)
		return;

	m_lightMask.clear(sf::Color::White);
	m_lightMaskBuffer.clear(sf::Color::White);

	m_shadowMaskBuffer.clear();

	for (auto& l : LightWorld::Get().m_Lights) {
		if (!l->Drawable || l->Radius <= 0.0f)
			continue;

		// TODO: check for bitshift
		m_spotShader.setUniform("u_attenuation"	, l->Attenuation <= 0.0f ? 0.0005f : l->Attenuation);
		m_spotShader.setUniform("u_lightPower"	, l->LightPower	);
		m_spotShader.setUniform("u_position"	, l->Position	);
		m_spotShader.setUniform("u_radius"		, l->Radius		);
		m_spotShader.setUniform("u_color"		, ColorToNormalizedVec3(l->Color));

		m_texturer.setTexture(m_lightMaskBuffer.getTexture());

		if (!shadows) {
			m_spotShader.setUniform("u_shadow", m_shadowMaskBuffer.getTexture());
			m_lightMask.draw(m_texturer, &m_spotShader);

			m_texturer.setTexture(m_lightMask.getTexture());

			m_lightMaskBuffer.draw(m_texturer);
			continue;
		}

		CreateShadowMask(*l);

		m_lightMaskBuffer.display();
		m_lightMask.display();

		m_spotShader.setUniform("u_shadow", m_shadowMaskBuffer.getTexture());
		m_lightMask.draw(m_texturer, &m_spotShader);

		m_texturer.setTexture(m_lightMask.getTexture());

		m_lightMaskBuffer.draw(m_texturer);
	}

	m_lightMask.display();

	changed = false;
}

void LightRenderer::CreateShadowMask(const Light& light) {
	CastRays(light.Position);

	m_ShadowVertexArray.clear();

	m_ShadowVertexArray.emplace_back(light.Position, sf::Color::Black);

	for (uint32_t i = 0; i < m_RayPoints.size(); i++)
		m_ShadowVertexArray.emplace_back(m_RayPoints[i].Position, sf::Color::Black);

	if (!m_RayPoints.empty())
		m_ShadowVertexArray.emplace_back(m_RayPoints[0].Position, sf::Color::Black);

	m_shadowMaskBuffer.clear(sf::Color::White);
	m_shadowMaskBuffer.draw(m_ShadowVertexArray.data(), m_ShadowVertexArray.size(), sf::TriangleFan);
	m_shadowMaskBuffer.display();
}

void LightRenderer::CastRays(sf::Vector2f pos) {
	m_Vertices.clear();
	m_RayPoints.clear();

	auto& edges = LightWorld::Get().m_Edges;

	for (auto& e : edges) {
		m_Vertices.insert(e->Start);
		m_Vertices.insert(e->End);
	}

	for (auto& v : m_Vertices) {
		sf::Vector2f rd = v - pos;

		float baseAngle = atan2f(rd.y, rd.x);

		float ang = 0.0f;

		for (uint32_t j = 0; j < 3; j++) {
			if (j == 0)	ang = baseAngle - 0.0005f;
			if (j == 1)	ang = baseAngle;
			if (j == 2)	ang = baseAngle + 0.0005f;

			rd.x = 1000.0f * cosf(ang);
			rd.y = 1000.0f * sinf(ang);

			sf::Vector2f minP;
			float		 minT1 = 9999.0f;
			float		 minAng = 0.0f;
			bool		 valid = false;

			for (auto& e : edges) {
				sf::Vector2f sd = e->End - e->Start;

				float t2 = (rd.x * (e->Start.y - pos.y) + (rd.y * (pos.x - e->Start.x))) / (sd.x * rd.y - sd.y * rd.x);

				float t1 = (e->Start.x + sd.x * t2 - pos.x) / rd.x;

				if (t1 > 0.0f && t2 >= 0.0f && t2 <= 1.0f) {
					if (t1 < minT1) {
						minT1 = t1;
						minP = pos + rd * t1;
						minAng = atan2f(minP.y - pos.y, minP.x - pos.x);

						valid = true;
					}
				}
			}

			if (valid)
				m_RayPoints.emplace_back(sf::Vector2f(minP.x, minP.y), minAng);
		}
	}

	std::sort(m_RayPoints.begin(), m_RayPoints.end(), [&](const Point& a, const Point& b) { return a.Angle < b.Angle; });
}

void LightRenderer::ResizeTextures(sf::Vector2u size) {
	{
		m_lightMask.create(size.x, size.y);
		m_lightMaskBuffer.create(size.x, size.y);
	}

	{
		m_shadowMask.create(size.x, size.y);
		m_shadowMaskBuffer.create(size.x, size.y);
	}
}

void LightRenderer::ResizeShaders(sf::Vector2f size)  {
	{
		m_spotShader.setUniform("u_resolution", size);
	}
}

void LightRenderer::ResizeShadowBox() {
	//TODO: rewrite
}

void LightRenderer::LoadShaders() {
	{
		m_spotShader.loadFromFile("temp/default.vert", "temp/spot.frag");
	}

	{
		m_mergeShader.loadFromFile("temp/default.vert", "temp/merge.frag");
		m_mergeShader.setUniform("u_level", LightWorld::Get().m_lightLevel);
	}
}

void LightRenderer::InitShadowBox() {
	sf::Vector2f center = Camera::Get().GetCenter();
	sf::Vector2f size = Camera::Get().GetSize();

	m_ShadowBox.push_back(new Edge({ sf::Vector2f(center   - size),						 sf::Vector2f(center.x + size.x, center.y - size.y) }));
	m_ShadowBox.push_back(new Edge({ sf::Vector2f(center.x + size.x, center.y - size.y), sf::Vector2f(center.x + size.x, center.y + size.y) }));
	m_ShadowBox.push_back(new Edge({ sf::Vector2f(center.x + size.x, center.y + size.y), sf::Vector2f(center.x - size.x, center.y + size.y) }));
	m_ShadowBox.push_back(new Edge({ sf::Vector2f(center.x - size.x, center.y + size.y), sf::Vector2f(center   - size)						}));

	auto& edges = LightWorld::Get().m_Edges;


	for (auto& b : m_ShadowBox)
		edges.insert(b);
}

void LightRenderer::DebugDraw(Renderer& renderer) {
	m_Lines.clear();

	const auto& flags = LightWorld::Get().m_flags;
	//TODO: maybe change? 
	if (flags > 4)
		for (auto& e : LightWorld::Get().m_Edges) {
			m_Lines.emplace_back(e->Start, EDGE_COLOR);
			m_Lines.emplace_back(e->End, EDGE_COLOR);
		}

	if(flags > 0 && flags < 5)
		for (auto& l : LightWorld::Get().m_Lights) {
			Renderer::DrawCircle(renderer, l->Position, l->Radius, l->Color, false);
			
			//TODO: this shit dosen work 
			//float attenuation = (1.0f - l->Attenuation) * l->LightPower;
			float attenuation = 1.0f - l->Attenuation;
			
			attenuation = attenuation <= 0.55f ? attenuation * attenuation * attenuation : attenuation * attenuation;


			if(attenuation > 0.0f && attenuation < 1.0f)
				Renderer::DrawCircle(renderer, l->Position, l->Radius * attenuation, l->Color, false);
		}

	if (flags > 4)
		renderer.draw(m_Lines.data(), m_Lines.size(), sf::Lines);
}
