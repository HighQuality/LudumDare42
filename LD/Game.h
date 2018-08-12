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
	i32 GetScore() const { return myScore; }

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
	bool myIsMousePressed = false;
	bool myShowMenu = true;
	bool myLost = false;
	sf::Sprite myLostOverlay;

	std::vector<Vec2> myMenuPositions;

	Vec2 myMenuPosition;

	f32 myGameSpeed = 1.f;
	i32 myScore = 0;
	i32 myHighScore = 0;
	i32 myMenuIndex = -1;
	std::vector<sf::String> myMenuOptions;
};

