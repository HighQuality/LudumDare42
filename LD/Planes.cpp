#include "pch.h"
#include "Planes.h"
#include "StaticResources.h"
#include "Game.h"

Planes::Planes(Game* aGame)
{
	myGame = aGame;

	Resize(256);

	Runway runway;
	runway.start = Vec2(352.f, 278.f);
	runway.end = Vec2(665.f, -30.f);
	runway.width = 90.f;
	myRunways.push_back(runway);

	for (i32 i = 0; i < 3; ++i)
		CreatePlane();
}

Planes::~Planes()
{
}

void Planes::Update(const f32 aDeltaTime)
{
	UpdateRunways();

	for (i32 i = 0; i < myMaxNumberPlanes; ++i)
	{
		if (!myOccupied[i])
			continue;

		FollowPath(i, aDeltaTime);
	}

	for (i32 i = 0; i < myMaxNumberPlanes; ++i)
	{
		if (!myOccupied[i])
			continue;

		myPosition[i] += myDirection[i] * mySpeed[i] * aDeltaTime;
	}

	for (i32 i = 0; i < myMaxNumberPlanes; ++i)
	{
		if (!myOccupied[i])
			continue;

		mySprite[i].setPosition(myPosition[i]);
		mySprite[i].setRotation(-atan2f(-myDirection[i].y, myDirection[i].x) / Pi * 180.f);
	}

	for (i32 i = 0; i < myMaxNumberPlanes; ++i)
	{
		if (!myOccupied[i])
			continue;
		
		NearPlane plane = FindNearestPlane(myPosition[i], i);
		if (plane.plane >= 0 && plane.distance < myPlaneRadius[i])
		{
			myGame->Lost((myPosition[i] + myPosition[plane.plane]) / 2.f);
		}
	}
}

void Planes::FollowPath(const i32 aIndex, const f32 aDeltaTime)
{
	PathData& data = myPathData[aIndex];

	if (data.currentWaypoint < data.waypoints.size())
	{
		Vec2 delta = data.waypoints[data.currentWaypoint] - myPosition[aIndex];
		f32 distance = delta.GetLength();

		if (distance > 3.f)
		{
			myDirection[aIndex] = delta / distance;
		}
		else
		{
			++data.currentWaypoint;
		}

		std::vector<sf::Vertex> buffer;
		buffer.resize(data.waypoints.size() - data.currentWaypoint);
		for (i32 i = 0; i < buffer.size(); ++i)
		{
			buffer[i].position = data.waypoints[data.currentWaypoint + i];
			buffer[i].color = sf::Color::White;
			buffer[i].texCoords = sf::Vector2f(0.f, 0.f);
		}
		data.visualization.create(buffer.size());
		data.visualization.update(buffer.data(), buffer.size(), 0);
	}
}

NearPlane Planes::FindNearestPlane(const Vec2& aPosition, const i32 aException /*= -1*/) const
{
	i32 foundPlane = -1;
	f32 minimumDistanceSquared = std::numeric_limits<f32>::max();
	for (i32 i = 0; i < myMaxNumberPlanes; ++i)
	{
		if (!myOccupied[i] || i == aException)
			continue;
		
		const f32 distanceSquared = Max(0.f, (aPosition - myPosition[i]).GetLengthSquared() - myPlaneRadius[i] * myPlaneRadius[i]);
		
		if (distanceSquared < minimumDistanceSquared)
		{
			minimumDistanceSquared = distanceSquared;
			foundPlane = i;
		}
	}
	
	NearPlane result;
	result.plane = foundPlane;
	result.distance = std::sqrtf(minimumDistanceSquared);
	return result;
}

void Planes::CalculateRunwayDistances(const i32 aRunwayIndex, const Vec2& aLocation, f32& aDistanceFromMiddle, f32& aDistanceFromStart)
{
	const Runway& runway = myRunways[aRunwayIndex];
	const Vec2 RunwayStart = runway.start;
	const Vec2 RunwayEnd = runway.end;
	const Vec2 RunwayDirection = (RunwayEnd - RunwayStart).GetNormalized();
	const Vec2 RunwayRight(-RunwayDirection.y, RunwayDirection.x);
	aDistanceFromStart = (aLocation - RunwayStart).Dot(RunwayDirection);
	aDistanceFromMiddle = abs((aLocation - RunwayStart).Dot(RunwayRight));
}

void Planes::UpdateRunways()
{
}

void Planes::Draw(sf::RenderTarget& aRender)
{
	for (i32 i = 0; i < myMaxNumberPlanes; ++i)
	{
		if (!myOccupied[i])
			continue;

		PathData& data = myPathData[i];
		if (data.visualization.getVertexCount() > 1)
			aRender.draw(data.visualization, 0, data.visualization.getVertexCount());
	}

	for (i32 i = 0; i < myMaxNumberPlanes; ++i)
	{
		if (!myOccupied[i])
			continue;
		aRender.draw(mySprite[i]);
	}

	if (myCurrentlyDraggingPlan >= 0)
	{
		sf::CircleShape c;
		c.setFillColor(sf::Color::Transparent);
		c.setOutlineThickness(1.f);
		c.setOutlineColor(sf::Color::Black);
		c.setPointCount(32);

		const f32 radius = myPlaneRadius[myCurrentlyDraggingPlan] * 2.f;
		c.setPosition(myPosition[myCurrentlyDraggingPlan] - sf::Vector2f(radius, radius));
		c.setRadius(radius);
		aRender.draw(c);
	}

	if (gDebugDrawing)
	{
		sf::CircleShape c;
		c.setFillColor(sf::Color::Transparent);
		c.setOutlineThickness(1.f);
		c.setOutlineColor(sf::Color::Magenta);
		c.setPointCount(10);

		for (i32 i = 0; i < myMaxNumberPlanes; ++i)
		{
			if (!myOccupied[i])
				continue;
			const f32 radius = myPlaneRadius[i];
			c.setPosition(myPosition[i] - sf::Vector2f(radius, radius));
			c.setRadius(radius);
			aRender.draw(c);
		}
	}
}

void Planes::BeginDrag(const Vec2& aDragPos)
{
	NearPlane plane = FindNearestPlane(aDragPos, -1);
	if (plane.plane >=0 && plane.distance <= 10.f)
	{
		myCurrentlyDraggingPlan = plane.plane;
		myPathData[myCurrentlyDraggingPlan].currentWaypoint = 0;
		myPathData[myCurrentlyDraggingPlan].waypoints.clear();
	}
}

void Planes::Dragging(const Vec2& aDragPos)
{
	if (myCurrentlyDraggingPlan >= 0)
	{
		Vec2 currentWaypoint = myPosition[myCurrentlyDraggingPlan];
		PathData& pathData = myPathData[myCurrentlyDraggingPlan];

		if (pathData.waypoints.size() > 0)
			currentWaypoint = pathData.waypoints.back();

		if (aDragPos.DistanceTo(currentWaypoint) > 10.f)
		{
			for (i32 iRunway = 0; iRunway < myRunways.size(); ++iRunway)
			{
				Runway& runway = myRunways[iRunway];
				f32 distanceFromMiddle;
				f32 distanceFromStart;
				CalculateRunwayDistances(iRunway, aDragPos, distanceFromMiddle, distanceFromStart);
				
				if (distanceFromMiddle > runway.width / 2.f)
				{
					runway.runwayTraceProgress = 0.f;
					continue;
				}

				if (runway.runwayTraceProgress == 0.f)
				{
					f32 previousDistanceFromMiddle;
					f32 previousDistanceFromStart;
					CalculateRunwayDistances(iRunway, currentWaypoint, previousDistanceFromMiddle, previousDistanceFromStart);
					// Skip runways we're not currently tracing if we've already 
					if (previousDistanceFromStart > 0.f)
						continue;
				}

				if (distanceFromStart >= runway.runwayTraceProgress)
				{
					runway.runwayTraceProgress = distanceFromStart;
					std::cout << distanceFromStart << std::endl;
				}
				else
				{
					runway.runwayTraceProgress = 0.f;
					continue;
				}
			}

			pathData.waypoints.push_back(aDragPos);
		}
	}
}

void Planes::EndDrag()
{
	myCurrentlyDraggingPlan = -1;

	for (i32 i = 0; i < myRunways.size(); ++i)
	{
		myRunways[i].runwayTraceProgress = 0.f;
	}
}

void Planes::Resize(const i32 aNewSize)
{
	CHECK(aNewSize > myMaxNumberPlanes);

	myMaxNumberPlanes = aNewSize;

	myOccupied.resize(aNewSize);
	myPosition.resize(aNewSize);
	myDirection.resize(aNewSize);
	mySpeed.resize(aNewSize);
	mySprite.resize(aNewSize);
	myPathData.resize(aNewSize);
	myPlaneRadius.resize(aNewSize);

	for (i32 i = myMaxNumberPlanes; i < aNewSize; ++i)
	{
		myOccupied[i] = false;
		myPlaneRadius[i] = 0.f;
		mySpeed[i] = 0.f;
	}
}

i32 Planes::CreatePlane()
{
	for (i32 i = 0; i < myOccupied.size(); ++i)
	{
		if (!myOccupied[i])
		{
			InitializePlane(i);
			return i;
		}
	}

	const i32 newIndex = myMaxNumberPlanes;
	Resize(myMaxNumberPlanes * 2);
	InitializePlane(newIndex);
	return newIndex;

}

void Planes::DeletePlane(i32 aIndex)
{
	if (aIndex == myCurrentlyDraggingPlan)
	{
		myCurrentlyDraggingPlan = -1;
	}
	myOccupied[aIndex] = false;
}

void Planes::InitializePlane(i32 aIndex)
{
	myOccupied[aIndex] = true;
	myPathData[aIndex].visualization.setPrimitiveType(sf::LineStrip);
	
	const f32 planeSize = 2.5f;

	sf::Sprite& sprite = mySprite[aIndex];
	sprite.setTexture(gResources->planeTexture);
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize()) * 0.5f);
	sprite.setScale(sf::Vector2f(planeSize, planeSize));

	myPlaneRadius[aIndex] = Vec2(sf::Vector2f(sprite.getTexture()->getSize())).GetLength() * 0.5f * 0.9f * (planeSize / 2.f);

	myPosition[aIndex].x = RandomRange(0.f, 640.f);
	myPosition[aIndex].y = RandomRange(0.f, 480.f);
	myDirection[aIndex] = Vec2(1.f, 0.f).GetRotated(RandomRange(0.f, 360.f));

	const f32 MinSpeed = 25.f;
	const f32 MaxSpeed = 50.f;
	mySpeed[aIndex] = RandomRange(MinSpeed, MaxSpeed);
}
