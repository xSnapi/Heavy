#pragma once

/*
	Collider.hpp
*/

#include <SFML/System/Vector2.hpp>
#include "Heavy Box2D.hpp"
#include <functional>

namespace hv {
	class RigidBody;

	namespace Constants {
		constexpr float PPM = 30.0f; // Pixel per meter
	}
}

namespace hv {
	class Collider {
	public:
		Collider();
		virtual ~Collider();

		virtual void SetTriggered(bool triggered);

		std::function<void()> Function;

	protected:
		b2FixtureDef i_fixtureDef;

		friend class RigidBody;
	};
}

namespace hv {
	struct BoxCollider : public Collider {
	public:
		BoxCollider();

		BoxCollider(sf::Vector2f size);

		~BoxCollider();

		void SetSize(sf::Vector2f size);

	private:
		b2PolygonShape m_shape;
	};
}

namespace hv {
	struct CircleCollider : public Collider {
	public:
		CircleCollider();

		CircleCollider(float radius);

		~CircleCollider();

		// Modifiers

		void SetRadius(float radius);

	private:
		b2CircleShape m_shape;
	};
}

namespace hv {
	struct EdgeCollider : public Collider {
	public:
		EdgeCollider();

		~EdgeCollider();

		// Modifiers
		void AddPoints(sf::Vector2f start, sf::Vector2f end);
	private:
		b2EdgeShape m_shape;
		b2Body* m_body;
	};
}

namespace hv {
	class ContactListener : public b2ContactListener {
	public:
		virtual void BeginContact(b2Contact* contact);
		virtual void EndContact(b2Contact* contact);
	};
}