#pragma once

/*
	Main class used by Heavy to run application

	~How to use~
	1. Create class inheriting runtime
	2. Create m_window in it's constructor
	3. Create functions FixedUpdate, Update, Render
*/

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/Window/Event.hpp>

#include <box2d/b2_world.h>

#include <mutex>
#include <thread>

#include "Times.hpp"
#include "Asset Loader.hpp"

#define USE_MULTITHREAD 1

namespace hv {
	class Runtime {
	public:
		Runtime();
		virtual ~Runtime();

		virtual void FixedUpdate()	= 0;
		virtual void Update()	    = 0;
		virtual void Render()	    = 0;

		void Run();

		static b2World PhysicsWorld;
	protected:
		sf::RenderWindow m_window;
		sf::Event m_event;

		sf::Color m_clearColor;

		double m_physicsStep = 0.01;
		double m_pet		 = 0.0; // physics elapsed time

		void SetFrameLimit(uint32_t limit);

	private:
		sf::Clock m_dtClock;
		sf::Clock m_fdtClock;

		uint32_t m_frameLimit = 0;

		bool m_focus = true;

		#if ENABLE_IMGUI
			bool m_updated  = false;
		#endif

		#if USE_MULTITHREAD
			std::mutex m_mutex;
			std::thread* m_rendererThread = nullptr;
			bool m_isRunning = true;
		#endif
		
		void InitPhysicsWorld();
		void HandleEvents();
		void InitRenderer();
		void Delay();
		void GLInit(); // Initializes openGL states
	};
}