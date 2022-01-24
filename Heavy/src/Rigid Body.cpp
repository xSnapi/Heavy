#include <hvpch.h>
#include "Rigid Body.hpp"

#include "Heavy Math.hpp"
#include "Physics World.hpp"

namespace hv {
	RigidBody::RigidBody() {
		b2BodyDef bodyDef;
		
		m_body = PhysicsWorld::Get().m_world.CreateBody(&bodyDef);
	}

	RigidBody::RigidBody(const RigidBody& other) {
		m_fixture = other.m_fixture;

		CopyBody(other);

		if (other.m_collider)
			SetCollider(*other.m_collider);
	}

	RigidBody::RigidBody(sf::Vector2f pos, BodyType type) {
		pos /= Constants::PPM;

		b2BodyDef bodyDef;
		bodyDef.position = pos;
		bodyDef.type = (b2BodyType)type;

		m_body = PhysicsWorld::Get().m_world.CreateBody(&bodyDef);
	}

	RigidBody::RigidBody(sf::Vector2f pos, BodyType type, const Collider& collider) {
		pos /= Constants::PPM;

		b2BodyDef bodyDef;
		bodyDef.position = pos;
		bodyDef.type = (b2BodyType)type;
		
		m_body = PhysicsWorld::Get().m_world.CreateBody(&bodyDef);

		SetCollider(collider);
	}

	RigidBody::~RigidBody() {
		PhysicsWorld::Get().m_world.DestroyBody(m_body);
	}

	RigidBody* RigidBody::operator=(const RigidBody& other) {
		if (m_body)
			PhysicsWorld::Get().m_world.DestroyBody(m_body);

		m_fixture = other.m_fixture;

		CopyBody(other);

		if(other.m_collider)
			SetCollider(*other.m_collider);

		return this;
	}

	void RigidBody::SetCollider(const Collider& collider) {
		/*if (m_fixture)
			m_body->DestroyFixture(m_fixture);*/

		m_collider = const_cast<Collider*>(&collider);

		m_fixture = m_body->CreateFixture(&collider.i_fixtureDef);

		if (m_fixture->IsSensor())
			m_body->SetSleepingAllowed(false);
	}

	void RigidBody::SetPosition(sf::Vector2f position) {
		position /= Constants::PPM;
		
		m_body->SetTransform(position, m_body->GetAngle());
	}

	void RigidBody::SetType(BodyType type) {
		m_body->SetType((b2BodyType)type);
	}

	void RigidBody::SetGravityScale(float scale) {
		m_body->SetGravityScale(scale);
	}

	void RigidBody::SetFixedRotation(bool fixed) {
		m_body->SetFixedRotation(fixed);
	}

	void RigidBody::SetLinearDrag(float drag) {
		m_body->SetLinearDamping(drag);
	}

	void RigidBody::SetLinearVelocity(sf::Vector2f velocity) {
		m_body->SetLinearVelocity(velocity);
	}

	void RigidBody::AddForce(sf::Vector2f velocity) {
		m_body->ApplyForceToCenter(velocity, true);
	}

	void RigidBody::SetRotation(float angle) {
		m_body->SetTransform(m_body->GetPosition(), (angle * HV_PI) / 180.0f);
	}

	sf::Vector2f RigidBody::GetPosition() const {
		return (sf::Vector2f)m_body->GetPosition() * Constants::PPM;
	}

	sf::Vector2f RigidBody::GetLinearVelocity() const {
		return m_body->GetLinearVelocity();
	}

	float RigidBody::GetRotation() const {
		return m_body->GetAngle() * (180.0f / HV_PI);
	}

	bool RigidBody::ContainsPoint(sf::Vector2f point) const {
		point /= Constants::PPM;

		for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
			if (f->TestPoint(point))
				return true;

		return false;
	}

	void RigidBody::CopyBody(const RigidBody& other) {
		const auto& body = other.m_body;

		b2BodyDef bodyDef;
		bodyDef.position		= body->GetPosition();
		bodyDef.angle			= body->GetAngle();
		bodyDef.type			= body->GetType();
		bodyDef.userData		= body->GetUserData();
		bodyDef.allowSleep		= body->IsSleepingAllowed();
		bodyDef.angularDamping	= body->GetAngularDamping();
		bodyDef.angularVelocity = body->GetAngularVelocity();
		bodyDef.awake			= body->IsAwake();
		bodyDef.bullet			= body->IsBullet();
		bodyDef.enabled			= body->IsEnabled();
		bodyDef.fixedRotation	= body->IsFixedRotation();
		bodyDef.gravityScale	= body->GetGravityScale();
		bodyDef.linearDamping	= body->GetLinearDamping();
		bodyDef.linearVelocity	= body->GetLinearVelocity();

		m_body = PhysicsWorld::Get().m_world.CreateBody(&bodyDef);
	}

}