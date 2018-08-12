#pragma once
class Planes;

class Game
{
public:
	Game();
	~Game();

	void Run();

	void Initialize();

	void Lost(const Vec2& aLocation);

	const Vec2& GetMousePosition() const { return myWorldMousePos; }

	void Score(const Vec2& aPosition);

private:
	void Update(const f32 aDeltaTime);
	void Draw(sf::RenderTarget& aTarget);
	void ProcessEvent(const class sf::Event& aEvent);
	
	void MousePressed();
	void MouseReleased();
	
	sf::RenderWindow myWindow;
	Vec2 myWindowMousePos;
	std::unique_ptr<Planes> myPlanes;
	Vec2 myWorldMousePos;
	sf::Sprite myBackground;
	bool isMousePressed = false;
	bool myLost = false;
	sf::Sprite myLostOverlay;
};

