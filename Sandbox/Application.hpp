#pragma once
#include <Heavy.hpp>

#include <SFML/Graphics.hpp>

#include "Rigid Body.hpp"
#include "Particle Emitter.hpp"

#include "Point Light.hpp"

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

	hv::PointLight m_light;
};