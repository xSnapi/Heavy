#pragma once
#include <Heavy.hpp>

#include <SFML/Graphics.hpp>

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

	hv::BoxCollider floorC;
	hv::RigidBody floor;

	std::vector<hv::RigidBody> rbs;
	hv::BoxCollider rbC;
};