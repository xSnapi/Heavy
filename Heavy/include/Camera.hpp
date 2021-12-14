#pragma once

/*
	Camera.hpp
	Singleton class responsible for camera and it's effects
*/

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Heavy Math.hpp>

namespace hv {
	class HeavyShader;

	class Camera {
	public:
		static Camera& Get();
		void Init(sf::RenderWindow* window);

		void Update();

		// Cammera shake effect
		void Shake(float time, int power);

		// Modifiers
		void Move(sf::Vector2f offset);

		// Accessors
		sf::Vector2f GetCenter() const;

	private:
		Camera();
		~Camera();

		struct ShakeContext 
		{
			sf::Vector2f Center;
			sf::Vector2f Pos[6];
			uint32_t CurrentPos = 0;

			void CreatePos(const int& power);
		};

		sf::View m_view;
		sf::RenderWindow* m_window = nullptr;

		float m_shakeElapsedTime = 0.0f;
		float m_shakeTime = 0.0f;

		static Camera s_instance;

		void ApplyEffects();
		void Shake();
		ShakeContext m_sc;

		void Resize(const sf::Vector2f& size);

		Mat4x4 Projection;

		friend class HeavyShader;
	};
}