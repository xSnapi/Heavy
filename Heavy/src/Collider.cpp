#include <hvpch.h>
#include "Collider.hpp"

#include "Physics World.hpp"
#include "Core.hpp"
#include <box2d/box2d.h>

using namespace hv;

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

void Collider::SetRestitution(float restitution) {
	i_fixtureDef.restitution = restitution;
}

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

//TODO: finish
EdgeCollider::EdgeCollider() {
	b2BodyDef bodyDef;

	m_body = PhysicsWorld::Get().m_world.CreateBody(&bodyDef);

	i_fixtureDef.shape		= &m_shape;
	i_fixtureDef.density	= 0.0f;
	i_fixtureDef.friction	= 0.5f;
}

EdgeCollider::~EdgeCollider() {
	PhysicsWorld::Get().m_world.DestroyBody(m_body);
}

void EdgeCollider::AddPoints(sf::Vector2f start, sf::Vector2f end) {
	m_shape.SetTwoSided(start / Constants::PPM, end / Constants::PPM);

	m_Fixtures.emplace_back(m_body->CreateFixture(&i_fixtureDef));
}

void EdgeCollider::DeletePoint(size_t index) {
	m_body->DestroyFixture(m_Fixtures[index]);

	m_Fixtures.erase(m_Fixtures.begin() + index);
}

void EdgeCollider::SetPoint(sf::Vector2f start, sf::Vector2f end, size_t index) {
	b2Fixture* list = m_body->GetFixtureList();

	DeletePoint(index);
	AddPoints(start, end);
}

void EdgeCollider::Clear() {
	for (auto& f : m_Fixtures)
		m_body->DestroyFixture(f);

	m_Fixtures.clear();
}

std::vector<b2Fixture*>& EdgeCollider::Raw() {
	return m_Fixtures;
}

void ContactListener::BeginContact(b2Contact* contact) {
	if(contact->GetFixtureA()->IsSensor())
		((Collider*)contact->GetFixtureA()->GetUserData().pointer)->OnTriggerStart();
}

void ContactListener::EndContact(b2Contact* contact) {
	if (contact->GetFixtureA()->IsSensor())
		((Collider*)contact->GetFixtureA()->GetUserData().pointer)->OnTriggerEnd();
}
