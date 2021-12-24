#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/b2_world.h>

/*
	Simple difference between light and physic world is that 
	Physics world is just abstraction of b2 World class
*/

namespace hv {
	class  RigidBody;
	class  Runtime;
	struct EdgeCollider;

	class PhysicsWorld {
	public:
		static PhysicsWorld& Get();
	private:
		PhysicsWorld();
		~PhysicsWorld();

		b2World m_world;

		void InitDebugDraw(sf::RenderWindow& window);

		friend class  RigidBody;
		friend class  Runtime;
		friend struct EdgeCollider;

		static PhysicsWorld s_instance;
	};
}