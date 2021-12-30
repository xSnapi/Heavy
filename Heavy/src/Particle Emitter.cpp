#include <hvpch.h>
#include "Particle Emitter.hpp"

#include "Heavy Math.hpp"
#include "Heavy Times.hpp"
#include "Random.hpp"
#include "Heavy Utilities.hpp"

constexpr uint32_t VERTICES_COUNT = 6;

// TODO: napis losowe inicjalizowanie particlow zeby nie bylo wsazystkich na raz

namespace hv {
	// Everything here can probably be closed in one hashmap but this way it is easier to 
	// debug, and performance should be the same
	ParticleEmitter::ParticleEmitter() {
		m_Particles.reserve(m_max);

		m_Vertices.reserve((size_t)m_max * VERTICES_COUNT);
	}

	ParticleEmitter::~ParticleEmitter() {

	}

	void ParticleEmitter::Update() {
		for (size_t i = 0; i < m_Particles.size(); i++) {
			auto& p = m_Particles[i];
			uint32_t qIndex = (uint32_t)i * VERTICES_COUNT;

			p.LifeRemaining -= dt;

			if (p.LifeRemaining <= 0.0f) {
				DeleteQuad(qIndex);
				m_Particles.erase(m_Particles.begin() + i);
				continue;
			}

			float life = p.LifeRemaining / p.LifeTime;

			TransformQuad(qIndex, life);

			p.Velocity = Lerp(p.Velocity * m_torque, p.Velocity, life);

			if (!m_shift) {
				sf::Vector3f color = Lerp(m_deathColor, m_birthColor, life);

				SetQuadColor(qIndex, Vec3ToColor(color, char(life * 255)));
				continue;
			}

			sf::Color color = HueShift(sf::Color(255, 0, 0), m_shiftClock.getElapsedTime().asSeconds());
			color.a = sf::Uint8(life * 255);

			SetQuadColor(qIndex, color);
		}
	}

	void ParticleEmitter::AddParticles() {
		for (size_t i = m_Particles.size(); i < m_max; i++) {
			m_Particles.push_back(Particle());

			InitParticle(m_Particles.back());
			AddQuad();
		}
	}

	void ParticleEmitter::SetPosition(sf::Vector2f pos) {
		m_pos = pos;
	}

	void ParticleEmitter::SetDirection(float x, float y) {
		m_dir = sf::Vector2f(x, y);
	}

	void ParticleEmitter::SetSpread(float spread) {
		m_spread = spread;
	}

	void ParticleEmitter::SetSize(float size) {
		m_size = size;
	}

	void ParticleEmitter::SetTorque(float torque) {
		m_torque = torque;
	}

	void ParticleEmitter::SetLifeTimeRange(float min, float max) {
		m_lifeTimeMin = min;
		m_lifeTimeMax = max;
	}

	void ParticleEmitter::SetVelocity(sf::Vector2f velocity) {
		m_velocity = velocity;
	}

	void ParticleEmitter::SetBirthColor(sf::Color color) {
		m_birthColor = ColorToVec3(color);
	}

	void ParticleEmitter::SetDeathColor(sf::Color color) {
		m_deathColor = ColorToVec3(color);
	}

	void ParticleEmitter::SetMax(uint32_t max) {
		if (max > m_max) {
			m_Particles.reserve(m_max);
			m_Vertices.reserve((size_t)m_max * VERTICES_COUNT);
		}

		m_max = max;
	}

	void ParticleEmitter::SetTexture(sf::Texture& texture) {
		m_texture = &texture;
	}

	void ParticleEmitter::SetHueShift(bool var) {
		m_shift = var;
	}

	void ParticleEmitter::AddQuad() {
		std::array<sf::Vertex, VERTICES_COUNT> vertices;
		float size = m_size / 2.0f;

		vertices[0] = sf::Vector2f(m_pos.x - size, m_pos.y - size);
		vertices[1] = sf::Vector2f(m_pos.x + size, m_pos.y - size);
		vertices[2] = sf::Vector2f(m_pos.x + size, m_pos.y + size);

		vertices[3] = sf::Vector2f(m_pos.x - size, m_pos.y - size);
		vertices[4] = sf::Vector2f(m_pos.x - size, m_pos.y + size);
		vertices[5] = sf::Vector2f(m_pos.x + size, m_pos.y + size);
		//TODO: FIX
		/*if (m_texture) {
			sf::Vector2f size = (sf::Vector2f)m_texture->getSize();

			vertices[0].texCoords = sf::Vector2f(0.0f,   0.0f  );
			vertices[1].texCoords = sf::Vector2f(0.0f,   size.x);
			vertices[2].texCoords = sf::Vector2f(size.x, size.x);
			vertices[3].texCoords = sf::Vector2f(size.x, 0.0f  );
		}*/

		sf::Transform t;
		t.rotate(m_Particles.back().Rotation, m_pos);

		for (auto& v : vertices) {
			v.color = !m_shift ? Vec3ToColor(m_birthColor) : HueShift(sf::Color(255, 0, 0), m_shiftClock.getElapsedTime().asSeconds());
			v.position = t.transformPoint(v.position);
			m_Vertices.push_back(v);
		}
	}

	void ParticleEmitter::DeleteQuad(size_t index) {
		for (size_t i = index; i < index + VERTICES_COUNT; i++)
			m_Vertices.erase(m_Vertices.begin() + index);
	}

	void ParticleEmitter::TransformQuad(size_t index, float life) {
		sf::Vector2f& velocity =  m_Particles[index / VERTICES_COUNT].Velocity;
		sf::Vector2f& position =  m_Particles[index / VERTICES_COUNT].Position;
		float		  rotation = -m_Particles[index / VERTICES_COUNT].Rotation;

		sf::Transform t;

		// Calculating particle scale has to be done this way because of matrix combination
		float scale = (GetQuadScale(index, rotation));

		t.scale(sf::Vector2f(1.0f / scale, 1.0f / scale), position);
		t.scale(sf::Vector2f(life, life), position);

		t.translate(velocity * dt);

		position += velocity * dt;

		for (size_t i = index; i < index + VERTICES_COUNT; i++)
			m_Vertices[i].position = t.transformPoint(m_Vertices[i].position);
	}

	void ParticleEmitter::SetQuadColor(size_t index, sf::Color color) {
		for (size_t i = index; i < index + VERTICES_COUNT; i++)
			m_Vertices[i].color = color;
	}

	float ParticleEmitter::GetQuadScale(size_t index, float rotation) const {
		sf::Transform t;
		t.rotate(rotation);
		float distance = t.transformPoint(m_Vertices[index + 1].position).x - t.transformPoint(m_Vertices[index].position).x;
		return distance / m_size;
	}

	void ParticleEmitter::InitParticle(Particle& p) {
		HV_DEBUG_ASSERT(m_lifeTimeMin < m_lifeTimeMax);

		// Initializes particle
		p.LifeTime = Random::Get().Float(m_lifeTimeMin, m_lifeTimeMax);
		p.LifeRemaining = p.LifeTime;

		p.Size = m_size;
		p.SizeRemaining = p.Size;

		p.Rotation = (float)Random::Get().Int(0, 89);
		p.Position = m_pos;

		sf::Vector2f dir = m_dir;

		dir.x = !dir.x ? Random::Get().Float(-m_spread, m_spread) : dir.x;
		dir.y = !dir.y ? Random::Get().Float(-m_spread, m_spread) : dir.y;

		p.Velocity = sf::Vector2f(dir.x * m_velocity.x, dir.y * m_velocity.y) * Random::Get().Float(0.85f, 1.25f);
	}

	void ParticleEmitter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (m_texture)
			states.texture = m_texture;

		target.draw(m_Vertices.data(), m_Vertices.size(), sf::Triangles, states);
	}
}