#include <hvpch.h>
#include "Particle Emitter.hpp"

#include "Heavy Math.hpp"
#include "Heavy Times.hpp"
#include "Heavy Utilities.hpp"
#include "Random.hpp"

using namespace hv;

ParticleEmitter::ParticleEmitter(sf::Vector2f position, Particle particle)
	: m_position(position)
	, m_particle(particle)
{
	Resize(m_limit);
}

ParticleEmitter::ParticleEmitter() {
	Resize(m_limit);
}

ParticleEmitter::~ParticleEmitter() {

}

void ParticleEmitter::Update() {
	for (size_t i = 0; i < m_Particles.size();) {
		auto& p = m_Particles[i];

		p.LifeTimeRemaining -= dt;

		p.Life = p.LifeTimeRemaining / p.LifeTime;

		if (p.Life <= 0.0f) {
			std::swap(p, m_Particles.back());
			m_Particles.pop_back();

			continue;
		}

		float scale = p.SizeTarget / Lerp(p.Size, p.SizeTarget, p.Life);
		p.Scale = sf::Vector2f(scale, scale);

		if(m_particle.MovementType == MovementType::Linear)
			p.Position += p.Direction * p.Velocity * dt;
		else
			p.Position += p.Direction * p.Velocity * dt;


		sf::Color color = Vec3ToColor
		(
			Lerp(ColorToVec3(p.DeathColor), ColorToVec3(p.BirthColor), p.Life), 
			uint8_t(Lerp(p.OpacityTarget, p.Opacity, p.Life) * 255.0f)
		);

		for (uint32_t j = 0; j < 6; j++)
			p.Vertices[j].color = color;

		i++;
	}
}

void ParticleEmitter::Emitt() {
	m_elapsedTime += dt;

	if (m_elapsedTime > m_spawnTime) {
		if(m_Particles.size() < m_limit)
			CreateParticle();

		m_elapsedTime -= m_spawnTime;
	}
}

void ParticleEmitter::SetParticleSpawnTime(float time) {
	m_spawnTime = time;
}

void ParticleEmitter::SetParticleLimit(uint32_t limit) {
	if (limit > m_limit)
		Resize(limit);

	m_limit = limit;
}

void ParticleEmitter::CreateParticle() {
	ParticleProp p;

	p.LifeTime			= Random::Get().Float(m_particle.LifeTimeMin, m_particle.LifeTimeMax);
	p.LifeTimeRemaining = p.LifeTime;

	p.Size			= m_particle.Size;
	p.SizeTarget    = m_particle.Size * m_particle.SizeScale;

	p.Rotation = Random::Get().Float(0.0f, 90.0f);

	p.Velocity = m_particle.Velocity;

	p.Position = m_position;

	p.Direction = m_particle.Direction;

	p.Direction.x = !p.Direction.x ? Random::Get().Float(-m_spread, m_spread) : p.Direction.x;
	p.Direction.y = !p.Direction.y ? Random::Get().Float(-m_spread, m_spread) : p.Direction.y;

	p.BirthColor = m_particle.BirthColor;
	p.DeathColor = m_particle.DeathColor;

	p.Opacity		= m_particle.Opacity;
	p.OpacityTarget = m_particle.OpacityTarget;

	auto& v = p.Vertices;
	auto  s = p.Size / 2.0f;

	if (!m_particle.Texture) {
		v[0] = sf::Vertex(sf::Vector2f(-s, -s));
		v[1] = sf::Vertex(sf::Vector2f( s, -s));
		v[2] = sf::Vertex(sf::Vector2f( s,  s));

		v[3] = sf::Vertex(sf::Vector2f(-s, -s));
		v[4] = sf::Vertex(sf::Vector2f(-s,  s));
		v[5] = sf::Vertex(sf::Vector2f( s,  s));
	}
	else {
		auto& t = (sf::Vector2f)m_particle.Texture->getSize();

		v[0] = sf::Vertex(sf::Vector2f(-s, -s), sf::Vector2f(0.0f, 0.0f));
		v[1] = sf::Vertex(sf::Vector2f( s, -s), sf::Vector2f(t.x,  0.0f));
		v[2] = sf::Vertex(sf::Vector2f( s,  s), sf::Vector2f(t.x,  t.y ));

		v[3] = sf::Vertex(sf::Vector2f(-s, -s), sf::Vector2f(0.0f, 0.0f));
		v[4] = sf::Vertex(sf::Vector2f(-s,  s), sf::Vector2f(0.0f, t.y ));
		v[5] = sf::Vertex(sf::Vector2f( s,  s), sf::Vector2f(t.x,  t.y ));
	}

	m_Particles.push_back(p);
}

void ParticleEmitter::Resize(size_t size) {
	m_Particles.reserve(size);
}

void ParticleEmitter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = m_particle.Texture;

	for (auto& p : m_Particles) {
		sf::Transform t;

		t.translate(p.Position);
		t.rotate(p.Rotation);
		t.scale(p.Scale);

		states.transform = t;

		target.draw(p.Vertices, 6, sf::Triangles, states);
	}
}