#include <hvpch.h>
#include "Event Dispatcher.hpp"

#pragma warning(disable : 4172)

//TODO: rewrite this pice of shit

namespace hv {
	void EventDispatcher::Init(sf::Event& event, bool& focus) { s_event = &event; s_focus = &focus; }

	// If window has focus is necessary for application not to get input when the window is minimized
	const sf::Event::EventType& EventDispatcher::Get() { sf::Event::EventType buffer = s_event->type;  if (!*s_focus) return (sf::Event::EventType)0; s_event->type = (sf::Event::EventType)0; return buffer; }
	#pragma warning(default : 4172) // Disables "returning empty reference" warning

	sf::Event* EventDispatcher::s_event;
	bool* EventDispatcher::s_focus;
}
