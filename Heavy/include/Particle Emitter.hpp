#pragma once

/*
	Particle Emitter.hpp
	Creates and emitts particles
*/

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

namespace hv {
	class ParticleEmitter : public sf::Drawable {
	public:
		ParticleEmitter();
		~ParticleEmitter();

		void Update();

		// Adds particles to emitter should be used close to update function
		void AddParticles();

		// Sets emitter position
		void SetPosition(sf::Vector2f pos);

		// Sets direction that particles are flying to
		void SetDirection(float x = 0.0f, float y = 0.0f);
		
		// When one of directionos is set to 0.0
		// we add spread to particles
		void SetSpread(float spread);
		
		// Sets particle size
		void SetSize(float size);

		// Slows the particle, 1.0 - no torque
		void SetTorque(float torque);
		
		// Time of particle life
		void SetLifeTimeRange(float min, float max);

		// Speed the particles move
		void SetVelocity(sf::Vector2f velocity);

		// Birth color of particle
		void SetBirthColor(sf::Color color);

		// Death color of particle
		void SetDeathColor(sf::Color color);

		// Sets max how many particles can be drawen at once
		void SetMax(uint32_t max);

		void SetTexture(sf::Texture& texture);

	private:
		struct Particle {
			float LifeTime		= 0.0f;
			float LifeRemaining = 0.0f;

			float Rotation		= 0.0f;

			float Size			= 0.0f;
			float SizeRemaining = 0.0f;

			sf::Vector2f Position;
			sf::Vector2f Velocity;
		};

		sf::Vector2f m_pos;
		sf::Vector2f m_dir;

		sf::Vector3f m_birthColor = sf::Vector3f(255, 0, 0);
		sf::Vector3f m_deathColor = sf::Vector3f(255, 255, 255);

		std::vector<sf::Vertex> m_Vertices;
		std::vector<Particle>   m_Particles;

		float m_lifeTimeMin = 1.0f;
		float m_lifeTimeMax = 3.0f;

		float m_size			= 50.0f;
		float m_spread			= 0.2f;
		float m_torque			= 0.988f;
		sf::Vector2f m_velocity = sf::Vector2f(100.0f, 100.0f);

		uint32_t m_max			= 99;

		sf::Texture* m_texture  = nullptr;

		void AddQuad();
		void DeleteQuad(size_t index);
		void TransformQuad(size_t index, float life);
		void SetQuadColor(size_t index, sf::Color color);

		float GetQuadScale(size_t index, float rotation) const;

		void InitParticle(Particle& particle);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}