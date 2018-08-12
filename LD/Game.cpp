#include "pch.h"
#include "Game.h"
#include "StaticResources.h"
#include "Planes.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Run()
{
	myWindow.create(sf::VideoMode(640, 480), "Plane Z");
	myWindow.setKeyRepeatEnabled(false);
	myWindow.setVerticalSyncEnabled(true);

	CHECK(!gResources && "Only one game instance is supported at a time");
	
	Initialize();
	
	sf::Clock stopwatch;
	f32 deltaTime = 1.f / 60.f;

	while (myWindow.isOpen())
	{
		const bool wasMousePressed = isMousePressed;

		sf::Event event;
		while (myWindow.pollEvent(event))
			ProcessEvent(event);
		
		if (wasMousePressed)
			myPlanes->Dragging(myWorldMousePos);

		Update(deltaTime);

		myWindow.clear(sf::Color(6, 158, 69));
		Draw(myWindow);
		myWindow.display();

		deltaTime = stopwatch.getElapsedTime().asSeconds();
		stopwatch.restart();
	}

	gResources.reset();
}

void Game::Initialize()
{
	gResources = std::make_unique<StaticResources>();

	myPlanes = std::make_unique<Planes>(this);
	myBackground.setTexture(gResources->backgroundTexture);
	myLostOverlay.setTexture(gResources->lostTexture);
	myLostOverlay.setOrigin(sf::Vector2f(gResources->lostTexture.getSize()) * 0.5f);
}

void Game::Lost(const Vec2& aLocation)
{
	myLostOverlay.setPosition(aLocation);
	myLost = true;
}

void Game::Score(const Vec2& aPosition)
{
	std::cout << "score" << std::endl;
}

void Game::ProcessEvent(const sf::Event& aEvent)
{
	switch (aEvent.type)
	{
	case sf::Event::Closed:
		myWindow.close();
		break;

	case sf::Event::MouseButtonPressed:
		if (aEvent.mouseButton.button == sf::Mouse::Button::Left && !isMousePressed)
		{
			isMousePressed = true;
			MousePressed();
		}
		break;

	case sf::Event::MouseButtonReleased:
		if (aEvent.mouseButton.button == sf::Mouse::Button::Left && isMousePressed)
		{
			isMousePressed = false;
			MouseReleased();
		}
		break;

	case sf::Event::MouseMoved:
		myWindowMousePos = Vec2(static_cast<f32>(aEvent.mouseMove.x), static_cast<f32>(aEvent.mouseMove.y));
		myWorldMousePos = sf::Vector2f(myWindow.mapPixelToCoords(sf::Vector2i(aEvent.mouseMove.x, aEvent.mouseMove.y)));
		break;

	case sf::Event::KeyPressed:
		if (aEvent.key.code == sf::Keyboard::Key::F2)
			gDebugDrawing = !gDebugDrawing;
		break;
	}
}

void Game::MousePressed()
{
	myPlanes->BeginDrag(myWorldMousePos);
}

void Game::MouseReleased()
{
	myPlanes->EndDrag();
}

void Game::Update(const f32 aDeltaTime)
{
	if (!myLost)
	{
		myPlanes->Update(aDeltaTime);
	}
}

void Game::Draw(sf::RenderTarget& aTarget)
{
	aTarget.draw(myBackground);
	myPlanes->Draw(aTarget);

	if (myLost)
		aTarget.draw(myLostOverlay);
}
