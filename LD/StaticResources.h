#pragma once

class StaticResources
{
public:
	const sf::Texture planeTexture = FromFile("textures/plane.png");
	const sf::Texture backgroundTexture = FromFile("textures/background.png");
	const sf::Texture backgroundArrowsTexture = FromFile("textures/background_arrows.png");
	const sf::Texture lostTexture = FromFile("textures/lost.png");
	const sf::Font arial = FontFromFile("fonts/arial.ttf");

private:
	sf::Texture FromFile(const char* aPath)
	{
		sf::Texture texture;
		texture.loadFromFile(aPath);
		return texture;
	}

	sf::Font FontFromFile(const char* aPath)
	{
		sf::Font font;
		font.loadFromFile(aPath);
		return font;
	}
};

extern std::unique_ptr<const StaticResources> gResources;
