#pragma once
#include <Heavy.hpp>

#include <SFML/Graphics.hpp>

#include "Light.hpp"
#include "Rigid Body.hpp"

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

	std::vector<hv::LightEdge> m_Edges;

	hv::SpotLight m_light0;
	hv::SpotLight m_light1;
};
