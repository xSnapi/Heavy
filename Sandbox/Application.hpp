#pragma once
#include <Heavy.hpp>

#include <SFML/Graphics.hpp>

#include "Animator.hpp"

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

	hv::Animation m_campfire;
	hv::Animator  m_animator;

	sf::Sprite m_campfireSprite;
};
