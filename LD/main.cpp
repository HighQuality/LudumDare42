#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	window.setKeyRepeatEnabled(false);
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	sf::Texture img;
	img.loadFromFile("picture.png");
	sf::Sprite spr;
	spr.setTexture(img);

	sf::Clock stopwatch;
	float deltaTime = 1.f / 60.f;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				sf::Vector2f fSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
				window.setView(sf::View(fSize / 2.f, fSize));
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				std::cout << event.key.code << " down" << std::endl;
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				std::cout << event.key.code << " up" << std::endl;
			}
		}

		window.clear();
		window.draw(shape);
		window.draw(spr);
		window.display();
	
		deltaTime = stopwatch.getElapsedTime().asSeconds();
		stopwatch.restart();
	}

	return 0;
}
