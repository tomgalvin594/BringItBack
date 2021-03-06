//////////////////////////////////////////////////////////////////////////////
//
// Hx
// --
//
// Plinth
//
// Copyright(c) 2015 M.J.Silk
//
// This software is provided 'as-is', without any express or implied
// warranty.In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
// M.J.Silk
// MJSilk2@gmail.com
//
//////////////////////////////////////////////////////////////////////////////

#include "Hx/Plinth/SFML/Generic.hpp"

namespace hx
{
	namespace Sfml
	{


// SFML from Hx

inline sf::Vector2f vector2(const hx::Vector2d& vector)
{
	return{ static_cast<float>(vector.x), static_cast<float>(vector.y) };
}

inline sf::Vector2i vector2(const hx::Vector2i& vector)
{
	return{ vector.x, vector.y };
}

inline sf::Vector2u vector2(const hx::Vector2u& vector)
{
	return{ vector.x, vector.y };
}

inline sf::Vector3f vector3(const hx::Vector3d& vector)
{
	return{ static_cast<float>(vector.x), static_cast<float>(vector.y), static_cast<float>(vector.z) };
}

inline sf::Vector3i vector3(const hx::Vector3i& vector)
{
	return{ vector.x, vector.y, vector.z };
}

inline sf::Vector3i vector3(const hx::Vector3u& vector)
{
	return{ static_cast<int>(vector.x), static_cast<int>(vector.y), static_cast<int>(vector.z) };
}


// Hx from SFML

inline hx::Vector2d vector2(const sf::Vector2f& vector)
{
	return{ vector.x, vector.y };
}

inline hx::Vector2i vector2(const sf::Vector2i& vector)
{
	return{ vector.x, vector.y };
}

inline hx::Vector2u vector2(const sf::Vector2u& vector)
{
	return{ vector.x, vector.y };
}

inline hx::Vector3d vector3(const sf::Vector3f& vector)
{
	return{ vector.x, vector.y, vector.z };
}

inline hx::Vector3i vector3(const sf::Vector3i& vector)
{
	return{ vector.x, vector.y, vector.z };
}














sf::Color colorFromValue(long int value)
{
	return colorFromRgb(hx::Color::rgbFromValue(value));
}

sf::Color colorFromRgb(const hx::Color::Rgb& rgb)
{
	return sf::Color(hx::Tween::linear(0, 255, rgb.r), hx::Tween::linear(0, 255, rgb.g), hx::Tween::linear(0, 255, rgb.b));
}

sf::Color colorBlend(const sf::Color& colorA, const sf::Color& colorB, double alpha)
{
	return sf::Color(hx::Tween::linear(colorA.r, colorB.r, alpha), hx::Tween::linear(colorA.g, colorB.g, alpha), hx::Tween::linear(colorA.b, colorB.b, alpha), hx::Tween::linear(colorA.a, colorB.a, alpha));
}

sf::Color colorBlend(const sf::Color& colorA, const sf::Color& colorB, float alpha)
{
	return colorBlend(colorA, colorB, static_cast<double>(alpha));
}

float dotProduct(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

float lengthSquared(const sf::Vector2f& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

float length(const sf::Vector2f& vector)
{
	return sqrt(lengthSquared(vector));
}

float lengthSquared(const sf::Vector3f& vector)
{
	return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

float length(const sf::Vector3f& vector)
{
	return sqrt(lengthSquared(vector));
}

sf::Vector3f crossProduct(const sf::Vector3f& a, const sf::Vector3f& b)
{
	return
	{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

/*
float crossProductPoints(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return{ a.x * b.y - b.x * a.y };
}
*/

bool CCW(const std::vector<sf::Vector2f>& points)
{
	if (points.size() != 3)
		return false;

	return (points[2].y - points[0].y) * (points[1].x - points[0].x) > (points[1].y - points[0].y) * (points[2].x - points[0].x);
}

// vector of (2) lines. line is a vector of (2) vector2fs (the points of the line).
// i.e. { { line1.x, line.y }, { line2.x, line2.y } }
bool doLinesIntersect(const std::vector<std::vector<sf::Vector2f>>& lines)
{
	if (lines.size() != 2)
		return false;
	if ((lines[0].size() != 2) || (lines[1].size() != 2))
		return false;

	return	(	CCW({ lines[0][0], lines[1][0], lines[1][1] }) !=
				CCW({ lines[0][1], lines[1][0], lines[1][1] })) &&
			(	CCW({ lines[0][0], lines[0][1], lines[1][0] }) !=
				CCW({ lines[0][0], lines[0][1], lines[1][1] }));
}

bool isPointInsidePolygon(const sf::Vector2f& point, const std::vector<sf::Vector2f>& polygonVertices)
{
	return isPointInsidePolygon(point, polygonVertices, boundingBox(polygonVertices));
}

bool isPointInsidePolygon(const sf::Vector2f& point, const std::vector<sf::Vector2f>& polygonVertices, const sf::FloatRect& boundingBox)
{
	if (!boundingBox.contains(point))
		return false;

	bool isInside{ false };
	sf::Vector2f pointOutsideBoundingBox{ boundingBox.left - 1, boundingBox.top - 1 };
	for (unsigned int v{ 0 }, w{ static_cast<unsigned int>(polygonVertices.size() - 1) }; v < polygonVertices.size(); w = v++)
	{
		if (doLinesIntersect({ { polygonVertices[v], polygonVertices[w] }, { pointOutsideBoundingBox, point } }))
			isInside = !isInside;
	}
	return isInside;
}

bool doClosedPolylinesIntersect(const std::vector<sf::Vector2f>& a, const std::vector<sf::Vector2f>& b)
{
	for (unsigned int l{ 0 }, m{ static_cast<unsigned int>(a.size() - 1) }; l < a.size(); m = l++)
	{
		for (unsigned int v{ 0 }, w{ static_cast<unsigned int>(b.size() - 1) }; v < b.size(); w = v++)
		{
			if (doLinesIntersect({ { a[l], a[m] }, { b[v], b[w] } }))
				return true;
		}
	}
	return false;
}

sf::FloatRect boundingBox(const std::vector<sf::Vector2f>& vertices)
{
	sf::FloatRect box;
	if (vertices.size() > 0)
	{
		box.left = vertices[0].x;
		box.top = vertices[0].y;
	}
	// store bottom right corner otherwise moving the top or left of the box would require also adjusting the width and height to compensate
	sf::Vector2f bottomRight{ vertices[0] };
	for (auto& vertex : vertices)
	{
		if (vertex.x < box.left)
			box.left = vertex.x;
		else if (vertex.x > bottomRight.x)
			bottomRight.x = vertex.x;
		if (vertex.y < box.top)
			box.top = vertex.y;
		else if (vertex.y > bottomRight.y)
			bottomRight.y = vertex.y;
	}
	box.width = bottomRight.x - box.left;
	box.height = bottomRight.y - box.top;

	return box;
}

void changeAlpha(sf::Color& color, unsigned int alpha)
{
	color.a = hx::clamp(alpha, 0u, 255u);
}

void changeAlpha(sf::Color& color, float alpha)
{
	changeAlpha(color, static_cast<double>(alpha));
}

void changeAlpha(sf::Color& color, double alpha)
{
	changeAlpha(color, hx::Tween::linear(0u, 255u, alpha));
}

sf::Color colorFromColorAndAlpha(sf::Color color, unsigned int alpha)
{
	color.a = hx::clamp(alpha, 0u, 255u);
	return color;
}

sf::Color colorFromColorAndAlpha(sf::Color color, float alpha)
{
	return colorFromColorAndAlpha(color, hx::Tween::linear(0u, 255u, alpha));
}

sf::Color colorFromColorAndAlpha(sf::Color color, double alpha)
{
	return colorFromColorAndAlpha(color, hx::Tween::linear(0u, 255u, alpha));
}

sf::Vector2f roundVector(const sf::Vector2f& vector)
{
	return{ static_cast<float>(round(vector.x)), static_cast<float>(round(vector.y)) };
}

sf::Vector3f roundVector(const sf::Vector3f& vector)
{
	return{ static_cast<float>(round(vector.x)), static_cast<float>(round(vector.y)), static_cast<float>(round(vector.z)) };
}

sf::Vector2f floorVector(const sf::Vector2f& vector)
{
	return{ static_cast<float>(floor(vector.x)), static_cast<float>(floor(vector.y)) };
}

sf::Vector3f floorVector(const sf::Vector3f& vector)
{
	return{ static_cast<float>(floor(vector.x)), static_cast<float>(floor(vector.y)), static_cast<float>(floor(vector.z)) };
}

sf::Vector2f ceilVector(const sf::Vector2f& vector)
{
	return{ static_cast<float>(ceil(vector.x)), static_cast<float>(ceil(vector.y)) };
}

sf::Vector3f ceilVector(const sf::Vector3f& vector)
{
	return{ static_cast<float>(ceil(vector.x)), static_cast<float>(ceil(vector.y)), static_cast<float>(ceil(vector.z)) };
}


	} // namespace Sfml
} // namespace hx