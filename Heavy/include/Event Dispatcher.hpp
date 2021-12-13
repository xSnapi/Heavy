#pragma once

/*
	Event Dispatcher.hpp
	Class giving access to last event
*/

#include <SFML/Window/Event.hpp>

namespace hv {
	class EventDispatcher {
	public:
		static void Init(sf::Event& event, bool& focus);

		// Returns const reference to event in Runtime.hpp
		// if window dosen't have focus returns 0
		static const sf::Event::EventType& Get();
	private:
		static sf::Event* s_event;
		static bool* s_focus;
	};
}