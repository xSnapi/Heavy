#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/b2_world.h>

namespace hv {
	class  RigidBody;
	class  Runtime;
	struct EdgeCollider;

	class PhysicsWorld {
	public:
		static PhysicsWorld& Get();

		void InitDebugDraw(sf::RenderWindow& window);

	private:
		PhysicsWorld();
		~PhysicsWorld();

		b2World m_world;

		static PhysicsWorld s_instance;

		friend class  RigidBody;
		friend class  Runtime;
		friend struct EdgeCollider;
	};
}