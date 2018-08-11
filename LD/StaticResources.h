#pragma once

class StaticResources
{
public:
	const sf::Texture planeTexture = FromFile("textures/plane.png");
	const sf::Texture backgroundTexture = FromFile("textures/background.png");
	const sf::Texture lostTexture = FromFile("textures/lost.png");

private:
	sf::Texture FromFile(const char* aPath)
	{
		sf::Texture texture;
		texture.loadFromFile(aPath);
		return texture;
	}
};

extern std::unique_ptr<const StaticResources> gResources;
