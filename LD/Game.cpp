#include "pch.h"
#include "Game.h"
#include "StaticResources.h"
#include "Planes.h"

Game::Game()
{
	myMenuPositions.push_back(Vec2(160.f, 120.f));
	myMenuPositions.push_back(Vec2(160.f * 3.f, 120.f));
	myMenuPositions.push_back(Vec2(160.f, 120.f * 3.f));
	myMenuPositions.push_back(Vec2(160.f * 3.f, 120.f * 3.f));

	myMenuPosition = myMenuPositions.back();
	myMenuOptions.emplace_back("Play");
	myMenuOptions.emplace_back("Quit");
}

Game::~Game()
{
}

void Game::Run()
{
	myWindow.create(sf::VideoMode(640, 480), "Crowded at the runway", sf::Style::Titlebar | sf::Style::Close);
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
		const bool wasMousePressed = myIsMousePressed;

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
	myLostOverlay.setTexture(gResources->lostTexture);
	myLostOverlay.setOrigin(sf::Vector2f(gResources->lostTexture.getSize()) * 0.5f);
}

void Game::Lost(const Vec2& aLocation)
{
	myLostOverlay.setPosition(aLocation);
	myLost = true;
	myShowMenu = true;

	if (myScore > myHighScore)
		myHighScore = myScore;
}

void Game::Score(const Vec2& aPosition)
{
	++myScore;
}

void Game::ProcessEvent(const sf::Event& aEvent)
{
	switch (aEvent.type)
	{
	case sf::Event::Closed:
		myWindow.close();
		break;

	case sf::Event::MouseButtonPressed:
		if (aEvent.mouseButton.button == sf::Mouse::Button::Left && !myIsMousePressed)
		{
			myIsMousePressed = true;
			MousePressed();
		}
		break;

	case sf::Event::MouseButtonReleased:
		if (aEvent.mouseButton.button == sf::Mouse::Button::Left && myIsMousePressed)
		{
			myIsMousePressed = false;
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

		case sf::Keyboard::Key::Escape:
			if (!myLost)
				myShowMenu = !myShowMenu;
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
	if (!myShowMenu)
	{
		myPlanes->BeginDrag(myWorldMousePos);
	}
	else if (myMenuIndex >= 0)
	{
		switch (myMenuIndex)
		{
			// Play
		case 0:
			myShowMenu = false;
			if (myLost)
			{
				myPlanes = std::make_unique<Planes>(this);
				myScore = 0;
			}
			myLost = false;
			break;

			// Quit
		case 1:
			myWindow.close();
			break;
		}
	}
}

void Game::MouseReleased()
{
	myPlanes->EndDrag();
}

void Game::Update(const f32 aDeltaTime)
{
	if (!myLost && !myShowMenu)
	{
		myPlanes->Update(aDeltaTime);
	}
}

void Game::Draw(sf::RenderTarget& aTarget)
{
	if (myPlanes->IsDragging())
		myBackground.setTexture(gResources->backgroundArrowsTexture);
	else
		myBackground.setTexture(gResources->backgroundTexture);
	aTarget.draw(myBackground);
	myPlanes->Draw(aTarget);

	if (myLost)
		aTarget.draw(myLostOverlay);
	{

		sf::Text t;
		t.setStyle(sf::Text::Style::Bold);
		t.setFillColor(sf::Color::White);
		t.setOutlineColor(sf::Color::Black);
		t.setOutlineThickness(1.f);
		t.setFont(gResources->arial);
		t.setCharacterSize(16);
		t.setPosition(sf::Vector2f(8.f, 8.f));
		t.setString(sf::String("Current score: ") + std::to_string(myScore));
		aTarget.draw(t);
		t.setPosition(sf::Vector2f(8.f, 8.f + 16.f));
		t.setString(sf::String("Highscore: ") + std::to_string(myHighScore));
		aTarget.draw(t);
		t.setPosition(sf::Vector2f(8.f, 8.f + 32.f));
		t.setString("1 - 7: Change game speed");
		aTarget.draw(t);
	}

	if (myShowMenu)
	{
		myMenuIndex = -1;

		sf::Text t;
		t.setStyle(sf::Text::Style::Bold);
		t.setFillColor(sf::Color::White);
		t.setOutlineColor(sf::Color::Black);
		t.setOutlineThickness(1.f);
		t.setFont(gResources->arial);
		t.setCharacterSize(16);

		f32 currentY = myMenuPosition.y;

		for (i32 i = 0; i < myMenuOptions.size(); ++i)
		{
			sf::String menu = myMenuOptions[i];
			
			t.setString(menu);
			sf::FloatRect localBounds = t.getLocalBounds();
			t.setPosition(sf::Vector2f(myMenuPosition.x - localBounds.width / 2.f, currentY));
			sf::FloatRect globalBounds = t.getGlobalBounds();

			if (globalBounds.contains(myWorldMousePos))
			{
				menu.insert(0, "<");
				menu += ">";
				t.setString(menu);
				sf::FloatRect localBounds = t.getLocalBounds();
				t.setPosition(sf::Vector2f(myMenuPosition.x - localBounds.width / 2.f, currentY));
				myMenuIndex = i;
			}

			aTarget.draw(t);
			currentY += localBounds.height + 8.f;
		}

		t.setString("Crowded at the runway");
		t.setCharacterSize(22);
		sf::FloatRect bounds = t.getLocalBounds();
		t.setPosition(myMenuPosition + Vec2(-bounds.width / 2.f, -50.f));
		aTarget.draw(t);

		t.setString("Use your mouse to guide planes onto the runway.");
		t.setCharacterSize(14);
		bounds = t.getLocalBounds();
		t.setPosition(sf::Vector2f(640.f / 2.f - bounds.width / 2.f, 480.f - 58.f));
		currentY += bounds.height + 4.f;
		aTarget.draw(t);

		t.setString("If two planes collide you lose.");
		bounds = t.getLocalBounds();
		t.setPosition(sf::Vector2f(640.f / 2.f - bounds.width / 2.f, 480.f - 40.f));
		currentY += bounds.height + 4.f;
		aTarget.draw(t);

		t.setString("Made by HighQuality in 48 hours for Ludum Dare 42: \"Running out of Space\"");
		bounds = t.getLocalBounds();
		t.setPosition(sf::Vector2f(640.f / 2.f - bounds.width / 2.f, 480.f - 22.f));
		currentY += bounds.height + 4.f;
		aTarget.draw(t);
	}
}
