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
	myWindow.create(sf::VideoMode(640, 480), "Plane Z", sf::Style::Titlebar | sf::Style::Close);
	myWindow.setKeyRepeatEnabled(false);
	myWindow.setVerticalSyncEnabled(true);

	CHECK(!gResources && "Only one game instance is supported at a time");
	
	Initialize();
	
	sf::Clock stopwatch;
	f32 deltaTime = 1.f / 60.f;

	f32 timeAccumulator = 1.f;
	f32 timeStep = 1.f / 150.f;

	while (myWindow.isOpen())
	{
		const bool wasMousePressed = isMousePressed;

		sf::Event event;
		while (myWindow.pollEvent(event))
			ProcessEvent(event);
		
		if (wasMousePressed)
			myPlanes->Dragging(myWorldMousePos);

		timeAccumulator += deltaTime * myGameSpeed;
		while (timeAccumulator >= timeStep)
		{
			Update(timeStep);
			timeAccumulator -= timeStep;
		}

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
		switch (aEvent.key.code)
		{
		case sf::Keyboard::Key::Num1:
			myGameSpeed = 1.f;
			break;
		case sf::Keyboard::Key::Num2:
			myGameSpeed = 2.f;
			break;
		case sf::Keyboard::Key::Num3:
			myGameSpeed = 3.f;
			break;
		case sf::Keyboard::Key::Num4:
			myGameSpeed = 4.f;
			break;
		case sf::Keyboard::Key::Num5:
			myGameSpeed = 5.f;
			break;
		case sf::Keyboard::Key::Num6:
			myGameSpeed = 6.f;
			break;
		case sf::Keyboard::Key::Num7:
			myGameSpeed = 7.f;
			break;

		case sf::Keyboard::Key::F2:
			gDebugDrawing = !gDebugDrawing;
			break;
		}

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
