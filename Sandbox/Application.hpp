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

	hv::PointLight l0;
	//hv::SpotLight l1;

	hv::LightEdge ed;

	std::vector<hv::LightEdge> edges;
};
