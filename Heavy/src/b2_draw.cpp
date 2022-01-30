#include <hvpch.h>
#include "b2_draw.hpp"

#include "Collider.hpp"
#include "Heavy Renderer.hpp"

using namespace hv;

static sf::Color COLLIDER_COLOR = sf::Color(64, 224, 36);

b2DebugDraw::b2DebugDraw() {

}

b2DebugDraw::~b2DebugDraw() {

}

void b2DebugDraw::SetWindow(sf::RenderWindow& window) {
	m_window = &window;
}

void b2DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

	glColor4f(color.r, color.g, color.b, 0.5f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < vertexCount; i++) {
		b2Vec2 v = vertices[i];
		glVertex2f(v.x * 30, v.y * 30);
	}
	glEnd();
}

void b2DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	sf::Vertex drawVertcies[6] =
	{
		{ vertices[0] * Constants::PPM, COLLIDER_COLOR},

		{ vertices[1] * Constants::PPM, COLLIDER_COLOR},

		{ vertices[2] * Constants::PPM, COLLIDER_COLOR},

		{ vertices[3] * Constants::PPM, COLLIDER_COLOR},

		{ vertices[0] * Constants::PPM, COLLIDER_COLOR},

		{ vertices[2] * Constants::PPM, COLLIDER_COLOR},
	};

	if(m_window)
		m_window->draw(drawVertcies, 6, sf::LineStrip);
}

void b2DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {
	/*const float k_segments = 16.0f;
	const int vertexCount = 16;
	const float k_increment = 2.0f * b2_pi / k_segments;
	float theta = 0.0f;

	glColor4f(color.r, color.g, color.b, 1);
	glBegin(GL_LINE_LOOP);
	GLfloat glVertices[vertexCount * 2];
	for (int32 i = 0; i < k_segments; ++i) {
		b2Vec2 v = center + radius * b2Vec2(cos(theta), sin(theta));
		glVertex2f(v.x * 30, v.y * 30);
		theta += k_increment;
	}
	glEnd();*/
}

void b2DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {
	if(m_window)
		Renderer::DrawCircle(*m_window, center * Constants::PPM, radius * Constants::PPM);
}

void b2DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	sf::Vertex line[2] =
	{
		{ p1 * Constants::PPM, COLLIDER_COLOR },
		{ p2 * Constants::PPM, COLLIDER_COLOR },
	};

	sf::Vertex quads[12] =
	{
		{ p1 * Constants::PPM + sf::Vector2f(-2.5f, -2.5f), COLLIDER_COLOR },
		{ p1 * Constants::PPM + sf::Vector2f( 2.5f, -2.5f), COLLIDER_COLOR },
		{ p1 * Constants::PPM + sf::Vector2f( 2.5f,  2.5f), COLLIDER_COLOR },

		{ p1 * Constants::PPM + sf::Vector2f(-2.5f, -2.5f), COLLIDER_COLOR },
		{ p1 * Constants::PPM + sf::Vector2f(-2.5f,  2.5f), COLLIDER_COLOR },
		{ p1 * Constants::PPM + sf::Vector2f( 2.5f,  2.5f), COLLIDER_COLOR },


		{ p2 * Constants::PPM + sf::Vector2f(-2.5f, -2.5f), COLLIDER_COLOR },
		{ p2 * Constants::PPM + sf::Vector2f( 2.5f, -2.5f), COLLIDER_COLOR },
		{ p2 * Constants::PPM + sf::Vector2f( 2.5f,  2.5f), COLLIDER_COLOR },

		{ p2 * Constants::PPM + sf::Vector2f(-2.5f, -2.5f), COLLIDER_COLOR },
		{ p2 * Constants::PPM + sf::Vector2f(-2.5f,  2.5f), COLLIDER_COLOR },
		{ p2 * Constants::PPM + sf::Vector2f( 2.5f,  2.5f), COLLIDER_COLOR },
	};

	if (m_window) {
		m_window->draw(line, 2, sf::Lines);
		m_window->draw(quads, 12, sf::Triangles);
	}
}

void b2DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
	glColor4f(color.r, color.g, color.b, 1);
	glPointSize(1.0f);
	glBegin(GL_POINTS);
	glVertex2f(p.x * 30, p.y * 30);
	glEnd();
}

void b2DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c) {
	glColor4f(c.r, c.g, c.b, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(aabb->lowerBound.x * 30, aabb->lowerBound.y * 30);
	glVertex2f(aabb->upperBound.x * 30, aabb->lowerBound.y * 30);
	glVertex2f(aabb->upperBound.x * 30, aabb->upperBound.y * 30);
	glVertex2f(aabb->lowerBound.x * 30, aabb->upperBound.y * 30);
	glEnd();
}

void b2DebugDraw::DrawTransform(const b2Transform& xf) {
	b2Vec2 p1 = xf.p, p2;
	const float k_axisScale = 0.0f;
	
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1, p2, b2Color(1, 0, 0));
	
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1, p2, b2Color(0, 1, 0));
}
