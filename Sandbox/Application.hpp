#pragma once
#include <Heavy.hpp>

#include <SFML/Graphics.hpp>

#include "Rigid Body.hpp"
#include "Particle Emitter.hpp"

class Application : public hv::Runtime {
public:
	Application();
	~Application();

private:
	virtual void FixedUpdate();
	virtual void Update();
	virtual void Render();

	void InitAssets();
	void InitWindow();

	sf::Clock m_clock;

	std::vector<hv::RigidBody> m_Rbs;
	hv::BoxCollider m_rigidBodyCollider;

	std::vector<sf::RectangleShape> m_Boxes;

	hv::RigidBody m_floor;
	hv::BoxCollider m_floorCollider;
	sf::RectangleShape m_floorShape;

	hv::ParticleEmitter m_emitter;
};