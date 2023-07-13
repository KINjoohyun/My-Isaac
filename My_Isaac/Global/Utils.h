#pragma once
class Utils
{
private:
	static std::random_device rd;
	static std::mt19937 gen;

public:
	static int RandomRange(int min, int max);
	static float RandomRange(float min, float max);

	static void SetOrigin(sf::Sprite& sprite, Origins origin);
	static void SetOrigin(sf::Text& text, Origins origin);
	static void SetOrigin(sf::Shape& shape, Origins origin);
	static void SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect);

	static float SqrMagnitude(const sf::Vector2f& vec);
	static float Magnitude(const sf::Vector2f& vec);
	static sf::Vector2f Normalize(const sf::Vector2f& vec);

	static float RandomValue(); //0.0f ~ 1.0f¸¦ return
	static sf::Vector2f RandomOnCircle(float radius);
	static sf::Vector2f RandomInCircle(float radius);
	static float Distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2);

	static float Angle(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
	static float Angle(const sf::Vector2f& dir);

	static float Clamp(const float& value, const float& min, const float& max);
	static sf::Vector2f Clamp(const sf::Vector2f& value, const sf::Vector2f& min, const sf::Vector2f& max);
	static float Lerp(const float& value, const float& a, const float& b, const bool& isClamp = true);
	static sf::Vector2f Lerp(const float& value, const sf::Vector2f& a, const sf::Vector2f& b, const bool& isClamp = true);
	static sf::Color Lerp(const float& value, const sf::Color& a, const sf::Color& b, const bool& isClamp = true);
};