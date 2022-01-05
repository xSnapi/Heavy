#include <hvpch.h>
#include "Event Dispatcher.hpp"

namespace hv {
	bool* EventDispatcher::s_focus;
	std::unordered_map<sf::Event::EventType, sf::Event> EventDispatcher::s_events;

	void EventDispatcher::Init(bool& focus) {
		s_focus = &focus;
	}

	void EventDispatcher::DispatchEvent(const sf::Event& event) {
		s_events.insert(std::make_pair(event.type, event));
	}

	bool EventDispatcher::CheckFor(sf::Event::EventType type) {
		return s_events.find(type) != s_events.end();
	}

	sf::Event EventDispatcher::GetEvent(sf::Event::EventType type) {
		return s_events[type];
	}

	void EventDispatcher::Clear() {
		s_events.clear();
	}
}
