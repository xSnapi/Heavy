#pragma once
#include <Heavy.hpp>

#include <SFML/Graphics.hpp>

#include "Light.hpp"

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

	hv::SpotLight m_light;

	hv::SpotLight m_light1;

	hv::SpotLight m_light2;
};