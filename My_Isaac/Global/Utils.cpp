#include "stdafx.h"
#include "Utils.h"

std::random_device Utils::rd;
std::mt19937 Utils::gen(Utils::rd());

int Utils::RandomRange(int min, int max)
{
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}
float Utils::RandomRange(float min, float max)
{
	std::uniform_real_distribution<> dist(min, max);
	return (float)dist(gen);
}

void Utils::SetOrigin(sf::Sprite& sprite, Origins origin)
{
	SetOrigin(sprite, origin, sprite.getLocalBounds());
}
void Utils::SetOrigin(sf::Text& text, Origins origin)
{
	SetOrigin(text, origin, text.getLocalBounds());
}
void Utils::SetOrigin(sf::Shape& shape, Origins origin)
{
	SetOrigin(shape, origin, shape.getLocalBounds());
}
void Utils::SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect)
{
	sf::Vector2f originPos(rect.width, rect.height);
	originPos.x *= (int)origin % 3 * 0.5f;
	originPos.y *= (int)origin / 3 * 0.5f;

	obj.setOrigin(originPos);
}

float Utils::SqrMagnitude(const sf::Vector2f& vec)
{
	return (vec.x * vec.x) + (vec.y * vec.y);
}
float Utils::Magnitude(const sf::Vector2f& vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}
sf::Vector2f Utils::Normalize(const sf::Vector2f& vec)
{
	if (Magnitude(vec) == 0.0f)
	{
		return vec;
	}
	return vec / Magnitude(vec);
}

float Utils::RandomValue()
{
	return RandomRange(0.0f, 1.0f);
}
sf::Vector2f Utils::RandomOnCircle(float radius)
{
	sf::Vector2f vec(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
	return Normalize(vec) * radius;
}
sf::Vector2f Utils::RandomInCircle(float radius)
{
	return RandomOnCircle(radius) * RandomValue();
}
float Utils::Distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2)
{
	return Magnitude(pos1 - pos2);
}

float Utils::Angle(const sf::Vector2f& pos1, const sf::Vector2f& pos2)
{
	return Angle(pos1 - pos2);
}
float Utils::Angle(const sf::Vector2f& dir)
{
	return atan2(dir.y, dir.x) * (180.0f / M_PI);
}

float Utils::Clamp(const float& value, const float& min, const float& max)
{
	return std::max(min, std::min(max, value));
	/* 위와 동일
	if (v < min)
	{
		return min;
	}
	if (v > max)
	{
		return max;
	}
	return v;
	*/
}
sf::Vector2f Utils::Clamp(const sf::Vector2f& value, const sf::Vector2f& min, const sf::Vector2f& max)
{
	return { Clamp(value.x, min.x, max.x) , Clamp(value.y, min.y, max.y) };
}
float Utils::Lerp(const float& value, const float& a, const float& b, const bool& isClamp)
{
	float result = b * value + (1.0f - value) * a;
	return (isClamp) ? Clamp(result, std::min(a, b), std::max(a, b)) : result;
}
sf::Vector2f Utils::Lerp(const float& value, const sf::Vector2f& a, const sf::Vector2f& b, const bool& isClamp)
{
	float v;
	if (isClamp)
	{
		v = Clamp(value, 0.0f, 1.0f);
	}
	return a + Normalize(b - a) * Distance(a, b) * v;
}
sf::Color Utils::Lerp(const float& value, const sf::Color& a, const sf::Color& b, const bool& isClamp)
{
	sf::Color result =
	{
		(sf::Uint8)Lerp(a.r, b.r, value, isClamp),
		(sf::Uint8)Lerp(a.g, b.g, value, isClamp),
		(sf::Uint8)Lerp(a.b, b.b, value, isClamp),
		(sf::Uint8)Lerp(a.a, b.a, value, isClamp)
	};
	return result;
}