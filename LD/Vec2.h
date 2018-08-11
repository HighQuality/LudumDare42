#pragma once

struct Vec2
{
	f32 x = 0.f;
	f32 y = 0.f;

	Vec2() = default;
	~Vec2() = default;

	Vec2(f32 aX, f32 aY)
	{
		x = aX;
		y = aY;
	}

	Vec2(const sf::Vector2f& aOther)
	{
		x = aOther.x;
		y = aOther.y;
	}

	Vec2 operator+(Vec2 aOther) const
	{
		return Vec2(x + aOther.x, y + aOther.y);
	}

	Vec2& operator+=(Vec2 aOther)
	{
		*this = *this + aOther;
		return *this;
	}

	Vec2 operator-(Vec2 aOther) const
	{
		return Vec2(x - aOther.x, y - aOther.y);
	}

	Vec2& operator-=(Vec2 aOther)
	{
		*this = *this - aOther;
		return *this;
	}

	Vec2 operator*(f32 aOther) const
	{
		return Vec2(x * aOther, y * aOther);
	}

	Vec2& operator*=(f32 aOther)
	{
		*this = *this * aOther;
		return *this;
	}

	Vec2 operator/(f32 aOther) const
	{
		return Vec2(x / aOther, y / aOther);
	}

	Vec2& operator/=(f32 aOther)
	{
		*this = *this / aOther;
		return *this;
	}

	operator sf::Vector2f() const
	{
		return sf::Vector2f(x, y);
	}

	f32 GetLength() const
	{
		return sqrtf(x * x + y * y);
	}

	f32 GetLengthSquared() const
	{
		return x * x + y * y;
	}

	Vec2 GetNormalized() const
	{
		const f32 length = GetLength();
		if (length == 0.f)
			return Vec2(1.f, 0.f);
		return *this / GetLength();
	}

	Vec2& Normalize()
	{
		*this = GetNormalized();
		return *this;
	}

	Vec2 GetRotated(const f32 aAngleDegrees) const
	{
		float sin = sinf(aAngleDegrees / 180.f * Pi);
		float cos = cosf(aAngleDegrees / 180.f * Pi);

		return Vec2(
			cos * x - sin * y,
			sin *x + cos * y
		);
	}

	Vec2& Rotate(const f32 aAngleDegrees)
	{
		*this = GetRotated(aAngleDegrees);
		return *this;
	}

	f32 DistanceTo(const Vec2& aOther) const
	{
		return (*this - aOther).GetLength();
	}
};
