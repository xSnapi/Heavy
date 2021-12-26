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

	hv::SpotLight l;
	//hv::SpotLight l2;

	hv::LightEdge edge;
	hv::LightEdge edge1;
	hv::LightEdge edge2;
	hv::LightEdge edge3;

	hv::LightEdge edge4;

	hv::RigidBody rb;
	hv::BoxCollider cl;
};
