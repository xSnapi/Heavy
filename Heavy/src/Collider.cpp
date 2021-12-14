#include <hvpch.h>
#include "Collider.hpp"

#include "Runtime.hpp"

#include "Core.hpp"
#include <box2d/box2d.h>

namespace hv {
	Collider::Collider() {
		i_fixtureDef.userData.pointer		= (uintptr_t)this;
		i_fixtureDef.density				= 1.0f;
		i_fixtureDef.friction				= 0.5f;
		i_fixtureDef.restitution			= 0.0f;
		i_fixtureDef.restitutionThreshold	= 0.5f;
		i_fixtureDef.isSensor				= false;
	}

	Collider::~Collider() {

	}

	void Collider::SetTriggered(bool triggered) {
		i_fixtureDef.isSensor = triggered;
	}
}

namespace hv {
	BoxCollider::BoxCollider() {
		i_fixtureDef.shape = &m_shape;

		//m_shape.m_radius = 0.005f;
	}

	BoxCollider::BoxCollider(sf::Vector2f size) {
		size /= 2.0f;
		size /= Constants::PPM;

		m_shape.SetAsBox(size.x, size.y);
		
		i_fixtureDef.shape = &m_shape;
	}

	BoxCollider::~BoxCollider() {

	}

	void BoxCollider::SetSize(sf::Vector2f size) {
		size /= 2.0f;
		size /= Constants::PPM;

		m_shape.SetAsBox(size.x, size.y);
	}
}

namespace hv {
	CircleCollider::CircleCollider() {
		i_fixtureDef.shape = &m_shape;
	}

	CircleCollider::CircleCollider(float radius) {
		radius /= Constants::PPM;

		m_shape.m_radius = radius;
		
		i_fixtureDef.shape = &m_shape;
	}

	CircleCollider::~CircleCollider() {

	}

	void CircleCollider::SetRadius(float radius) {
		radius /= Constants::PPM;

		m_shape.m_radius = radius;
	}
}

namespace hv {
	//TODO: finish
	EdgeCollider::EdgeCollider() {
		b2BodyDef bodyDef;

		m_body = Runtime::PhysicsWorld.CreateBody(&bodyDef);

		i_fixtureDef.shape		= &m_shape;
		i_fixtureDef.density	= 0.0f;
		i_fixtureDef.friction	= 0.5f;
	}

	EdgeCollider::~EdgeCollider() {
		Runtime::PhysicsWorld.DestroyBody(m_body);
	}

	void EdgeCollider::AddPoints(sf::Vector2f start, sf::Vector2f end) {
		m_shape.SetTwoSided(start / Constants::PPM, end / Constants::PPM);

		m_body->CreateFixture(&i_fixtureDef);
	}
}

namespace hv {
	void ContactListener::BeginContact(b2Contact* contact) {
		if(contact->GetFixtureA()->IsSensor())
			((Collider*)contact->GetFixtureA()->GetUserData().pointer)->Function();
	}

	void ContactListener::EndContact(b2Contact* contact) {

	}
}