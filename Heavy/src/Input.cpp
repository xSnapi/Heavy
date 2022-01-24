#include <hvpch.h>
#include "Input.hpp"

namespace hv {
	bool Input::Keyboard::KeyCheck(sf::Keyboard::Key key, bool repeat) {
		bool pressed = sf::Keyboard::isKeyPressed(key);

		if (repeat)
			return pressed;

		if (!s_Keys[key] && pressed) {
			s_Keys[key] = true;
			return true;
		}

		return false;
	}

	bool Input::Mouse::KeyCheck(sf::Mouse::Button key, bool repeat) {
		bool pressed = sf::Mouse::isButtonPressed(key);

		if (repeat)
			return pressed;

		if (!s_Buttons[key] && pressed) {
			s_Buttons[key] = true;
			return true;
		}

		return false;
	}

	sf::Vector2f Input::Mouse::GetRelativePosition() {
		if (!s_window->getSystemHandle())
			return { };

		return sf::Vector2f(sf::Mouse::getPosition(*s_window));
	}

	sf::Vector2f Input::Mouse::GetRelativeCoords() {
		if (!s_window->getSystemHandle())
			return { };

		return s_window->mapPixelToCoords(sf::Mouse::getPosition(*s_window), s_renderer->getView());
	}

	sf::Vector2f Input::Mouse::GetPosition() {
		if (!s_window->getSystemHandle())
			return { };

		return (sf::Vector2f)sf::Mouse::getPosition();
	}

	sf::Vector2f Input::Mouse::GetCoords() {
		if (!s_window->getSystemHandle())
			return { };

		return s_renderer->mapPixelToCoords(sf::Mouse::getPosition());
	}

	void Input::Init(sf::RenderWindow& window, Renderer& renderer) {
		s_window   = &window;
		s_renderer = &renderer;

		// Keyboard init
		for (int i = -1; i < sf::Keyboard::KeyCount; i++)
			s_Keys[(sf::Keyboard::Key)i] = false;

		// Mouse init
		for (int i = 0; i < sf::Mouse::ButtonCount; i++)
			s_Buttons[(sf::Mouse::Button)i] = false;
	}

	void Input::Update() {
		for (auto& key : s_Keys)
			if (!sf::Keyboard::isKeyPressed(key.first))
				key.second = false;

		for (auto& button : s_Buttons)
			if(!sf::Mouse::isButtonPressed(button.first))
				button.second = false;
	}

	void Input::BlockInput() {
		for (auto& key : s_Keys)
			key.second = true;

		for (auto& button : s_Buttons)
			button.second = true;
	}

	std::unordered_map<sf::Keyboard::Key, bool> Input::s_Keys;
	std::unordered_map<sf::Mouse::Button, bool> Input::s_Buttons;
	const sf::RenderWindow* Input::s_window;
	const Renderer*			Input::s_renderer;
}
