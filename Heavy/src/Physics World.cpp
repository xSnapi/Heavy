#include <hvpch.h>
#include "Physics World.hpp"

#include "Collider.hpp"
#include "b2_draw.hpp"

using namespace hv;

PhysicsWorld& PhysicsWorld::Get() { return s_instance; }

void PhysicsWorld::InitDebugDraw(sf::RenderWindow& window) {
	m_debugDraw.SetWindow(window);

	m_world.SetDebugDraw(&m_debugDraw);
}

uint32_t PhysicsWorld::GetB2DrawType(DebugDrawType type) const {
	switch (type) {

	case hv::DebugDrawType::Collider:
		return b2Draw::e_shapeBit;

	case hv::DebugDrawType::Joint:
		return b2Draw::e_jointBit;

	case hv::DebugDrawType::AABB:
		return b2Draw::e_aabbBit;

	case hv::DebugDrawType::MassCenter:
		return b2Draw::e_centerOfMassBit;
	}

	return 0;
}

static ContactListener Listener;

void PhysicsWorld::DisableDebugDraw() {
	m_debugDraw.SetFlags(0);

	m_debugDrawEnabled = false;
}

PhysicsWorld::PhysicsWorld() :
	m_world(sf::Vector2f(0.0f, 10.0f))
{
	m_world.SetContactListener(&Listener);
}

PhysicsWorld::~PhysicsWorld() {

}

PhysicsWorld PhysicsWorld::s_instance;
