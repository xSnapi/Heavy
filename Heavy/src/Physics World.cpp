#include <hvpch.h>
#include "Physics World.hpp"

#include "Collider.hpp"
#include "b2_draw.hpp"

namespace hv {
	PhysicsWorld& PhysicsWorld::Get() { return s_instance; }

	void PhysicsWorld::InitDebugDraw(sf::RenderWindow& window) {
		#if ENABLE_COLLIDER_DRAW
				// This memory leak is here on purpose
				// we don't have to make draw static or member variable
				// leaving it in memory will work just fine + we don't need access to it (for now)
				// TODO: CHANGE
				b2DebugDraw* draw = new b2DebugDraw(window);
				m_world.SetDebugDraw(draw);
		
				uint32_t flags = 0;
				flags += b2Draw::e_shapeBit;
		
				draw->SetFlags(flags);
		#endif
	}

	static ContactListener Listener;

	PhysicsWorld::PhysicsWorld() :
		m_world(sf::Vector2f(0.0f, 10.0f))
	{
		m_world.SetContactListener(&Listener);
	}

	PhysicsWorld::~PhysicsWorld() {

	}

	PhysicsWorld PhysicsWorld::s_instance;
}