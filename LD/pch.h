#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <functional>
#include <stdint.h>
#include <intrin.h>
#include <random>
#include <type_traits>
#include <sstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using u8 = uint8_t;
using i8 = int8_t;

using u16 = uint16_t;
using i16 = int16_t;

using u32 = uint32_t;
using i32 = int32_t;

using u64 = uint32_t;
using i64 = int32_t;

using f32 = float;
using f64 = double;

constexpr f32 Pi = 3.14159265358979323846f;
constexpr f32 TwoPi = Pi * 2.f;
constexpr f32 HalfPi = Pi / 2.f;
constexpr f32 QuarterPi = Pi / 4.f;
constexpr f32 InvPi = 1.f / Pi;

constexpr f32 Tau = TwoPi;


#define CHECK(e) do { if (!(e)) { std::cout << #e << " failed (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; __debugbreak(); abort(); } } while (false)

#include "Vec2.h"

inline bool gDebugDrawing = false;

inline std::random_device gRd;
inline std::mt19937 gRdGen(gRd());

__forceinline i32 RandomRange(i32 aMinInclusive, i32 aMaxInclusive)
{
	std::uniform_int_distribution<i32> dis(aMinInclusive, aMaxInclusive);
	return dis(gRdGen);
}

__forceinline f32 RandomRange(f32 aMinInclusive, f32 aMaxInclusive)
{
	std::uniform_real_distribution<f32> dis(aMinInclusive, aMaxInclusive);
	return dis(gRdGen);
}

__forceinline f32 RandomFloat()
{
	return RandomRange(0.f, 1.f);
}

template <typename T>
__forceinline T Min(T a, T b)
{
	if (a < b)
		return a;
	return b;
}

template <typename T>
__forceinline T Max(T a, T b)
{
	if (a > b)
		return a;
	return b;
}

template <typename T>
__forceinline T Clamp(T aValue, T aMin, T aMax)
{
	return Max(aMin, Min(aMax, aValue));
}
