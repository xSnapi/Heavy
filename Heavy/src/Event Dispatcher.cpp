#include <hvpch.h>
#include "Event Dispatcher.hpp"

#pragma warning(disable : 4172)

namespace hv {
	void EventDispatcher::Init(sf::Event& event, bool& focus) { s_event = &event; s_focus = &focus; }

	// If window has focus is necessary for application not to get input when the window is minimized
	const sf::Event::EventType& EventDispatcher::Get() { if (!*s_focus) return (sf::Event::EventType)0; return s_event->type; }
	#pragma warning(default : 4172) // Disables "returning empty reference" warning

	sf::Event* EventDispatcher::s_event;
	bool* EventDispatcher::s_focus;
}
