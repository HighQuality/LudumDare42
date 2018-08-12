#pragma once

class Game;

struct PathData
{
	std::vector<Vec2> waypoints;
	sf::VertexBuffer visualization;
	std::vector<sf::Vertex> vertices;
	i32 currentWaypoint = 0;
	i32 runwayPathIndex = -1;
};

struct NearPlane
{
	f32 distance = 0.f;
	i32 plane = -1;
};

struct Runway
{
	Vec2 start;
	Vec2 end;
	f32 width = 0.f;
	f32 runwayTraceProgress = 0.f;
};

class Planes
{
public:
	Planes(Game* aGame);
	~Planes();

	void Update(const f32 aDeltaTime);
	void Draw(sf::RenderTarget& aRender);

	void BeginDrag(const Vec2& aDragPos);
	void Dragging(const Vec2& aDragPos);
	void EndDrag();

private:
	void Resize(const i32 aNewSize);
	i32 CreatePlane();
	void DeletePlane(i32 aIndex);

	void InitializePlane(i32 aIndex);
	void FollowPath(const i32 aIndex, const f32 aDeltaTime);

	NearPlane FindNearestPlane(const Vec2& aPosition, const i32 aException = -1) const;
	
	void CalculateRunwayDistances(const i32 aRunwayIndex, const Vec2& aLocation, f32& aDistanceFromMiddle, f32& aDistanceFromStart);

	i32 myMaxNumberPlanes = 0;

	i32 myCurrentlyDraggingPlan = -1;

	std::vector<bool> myOccupied;
	std::vector<Vec2> myPosition;
	std::vector<Vec2> myDirection;
	std::vector<f32> mySpeed;
	std::vector<f32> myPlaneRadius;
	std::vector<f32> myLandingProgress;
	std::vector<f32> myPlaneSize;
	std::vector<sf::Sprite> mySprite;
	std::vector<sf::Sprite> myShadowSprite;
	std::vector<PathData> myPathData;

	std::vector<Runway> myRunways;

	Game* myGame = nullptr;
	void UpdateRunways();
};
