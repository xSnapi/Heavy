#include <SFML/Graphics.hpp>

#include "box2d/box2d.h"
int main() {
	sf::RenderWindow window(sf::VideoMode(800, 800), "SANDBOX");
	
	sf::Clock deltaClock;

	float deltaTime = 0.0f;

	b2World word(sf::Vector2f(0.0f, 10.0f));

	while (window.isOpen()) {
		sf::Event event;

		while(window.pollEvent(event)){
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		window.clear();
		window.display();
	}

	return 0;
}