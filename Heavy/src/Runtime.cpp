#include <hvpch.h>
#include "Runtime.hpp"

#include "Event Dispatcher.hpp"
#include "Heavy Debug.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Heavy ImGui.hpp"
#include "Asset Manager.hpp"
#include "Physics World.hpp"
#include "Light World.hpp"

namespace hv {
	Runtime::Runtime() 
		: m_event()
	{
		EventDispatcher::Init(m_focus);
		Input::Init(m_window);

		#if !DISABLE_CONSOLE
			system("cls"); // <- cls makes colored output work
		#endif
	}

	Runtime::~Runtime() {
		// Clearing asset libraries before exiting to make sure SFML won't try deleting them after destroying context
		hv::TextureLibrary::Get().Clear();
		hv::FontLibrary   ::Get().Clear();
		hv::ShaderLibrary ::Get().Clear();
		hv::SoundLibrary  ::Get().ClearSounds();
		hv::SoundLibrary  ::Get().ClearMusics();

		#if USE_MULTITHREAD
			delete m_rendererThread;
		#endif

		#if ENABLE_IMGUI
			ImGui::SFML::Shutdown();
		#endif
	}

	void Runtime::Run() {
		GLInit();

		HV_DEBUG_ASSERT(m_window.getSystemHandle()); // Window wasn't initialized before first update

		// Singletons initialization
		Camera		::Get().Init(&m_window);
		LightWorld	::Get().Init(m_window.getSize());
		PhysicsWorld::Get().InitDebugDraw(m_window);

		// Renderer initialization
		m_renderer.Resize(m_window.getSize());

		#if ENABLE_IMGUI
			ImGui::SFML::Init(m_window);
		#endif

		#if USE_MULTITHREAD
			m_window.setActive(false);
			InitRenderer();
		#endif

		while (m_window.isOpen()) {
			HV_PROFILE_FRAME("MainThread");
			
			m_pet += Times::Instance.DT;
			while (m_pet >= m_physicsStep) {
				Times::Instance.FDT = m_fdtClock.restart().asSeconds();
				if (Times::Instance.FDT >= m_physicsStep * 5.0f)
					Times::Instance.FDT = (float)m_physicsStep;

				#if USE_MULTITHREAD
					m_mutex.lock();
					
					FrameFixedUpdate();

					m_mutex.unlock();
				#else
					FrameFixedUpdate();
				#endif

				m_pet -= m_physicsStep;
			}

			m_focus = m_window.hasFocus();
			HandleEvents();

			#if USE_MULTITHREAD
				m_mutex.lock();

				FrameUpdate();

				m_mutex.unlock();

				Delay();
			#else
				FrameUpdate();

				RendererDraw();

				m_window.display();
			#endif

			EventDispatcher::Clear();

			if(m_focus)
				Input::Update();

			Times::Instance.DT = m_dtClock.restart().asSeconds();
			if (dt >= 0.5f)
				Times::Instance.DT = 0.03f;
		}
	}

	void Runtime::FrameFixedUpdate() {
		// Physics step settings
		constexpr uint32_t velocityCorrection = 12;
		constexpr uint32_t positionCorrection = 8;

		FixedUpdate();

		PhysicsWorld::Get().m_world.Step((float)m_physicsStep + 0.01f, velocityCorrection, positionCorrection);
	}

	void Runtime::FrameUpdate() {
		#if ENABLE_IMGUI
			ImGui::SFML::Update(m_window, sf::Time(sf::seconds(dt)));
			m_updated = true;
		#endif

		Update();

		Camera::Get().Update();

		if (LightWorld::Get().LightEnabled())
			LightWorld::Get().Update();
	}

	void Runtime::RendererDraw() {
		m_renderer.clear();

		Render();

		m_renderer.display();

		// Drawing Heavy components
		if (!LightWorld::Get().LightEnabled()) {
			m_window.draw(m_renderer.GetFrame());
		}
		else {
			LightWorld::Get().RenderLights(m_renderer);

			m_window.draw(m_renderer.GetFrame());
		}

		#if ENABLE_COLLIDER_DRAW
			PhysicsWorld::Get().m_world.DebugDraw();
		#endif
		
		#if ENABLE_IMGUI
			if (m_updated) // ImGui can't be rendered before first update (i hate multithreading)
				ImGui::SFML::Render(m_window);
		#endif
	}

	void Runtime::HandleEvents() {
		while (m_window.pollEvent(m_event)) {

			#if ENABLE_IMGUI
				ImGui::SFML::ProcessEvent(m_event);
			#endif

			EventDispatcher::DispatchEvent(m_event);

			switch (m_event.type) {
			case sf::Event::Closed:
			
				#if USE_MULTITHREAD
					m_isRunning = false;
					m_rendererThread->join();
				#endif

				m_window.close();
				break;

			case sf::Event::LostFocus:
				Input::BlockInput();
				break;

			case sf::Event::Resized:
				m_renderer.Resize(m_window.getSize());
				LightWorld::Get().Resize(m_window.getSize());
				break;
			}
		}
	}

#if USE_MULTITHREAD
	void Runtime::InitRenderer() {
		m_rendererThread = new std::thread([&]() {
			HV_PROFILE_THREAD("RendererThread");

			m_window.setActive(true);

			while (m_isRunning) {
				m_mutex.lock();

				RendererDraw();

				m_mutex.unlock();

				m_window.display();
			}
			
			Debug::Log(Color::Green, "[Renderer Exited successfully]\n");
		});
	}
#endif

	void Runtime::SetFrameLimit(uint32_t limit) {
		m_frameLimit = limit;
		m_window.setFramerateLimit(limit);
	}

	void Runtime::Delay(){
		if (m_frameLimit != 0)
			sf::sleep(sf::seconds((float)(1.0 / m_frameLimit))); // This here must be double converted to float otherwise application will blow up
	}

	void Runtime::GLInit() {
		glLineWidth(1.0f);
		glPointSize(2.0f);
	}
}