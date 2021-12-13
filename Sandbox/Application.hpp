#pragma once
#include <Heavy.hpp>

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

	hv::EdgeCollider collider;
	hv::ParticleEmitter m_emitter;
};