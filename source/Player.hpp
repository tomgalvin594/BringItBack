#ifndef BRINGITBACK_PLAYER_HPP
#define BRINGITBACK_PLAYER_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <Hx/Plinth/Generic.hpp>
#include <Hx/Plinth/SFML/ExtendedColorList.hpp>

//#include <SFML/Graphics/ConvexShape.hpp>


class Player : public sf::Drawable, public sf::Transformable
{
public:
	Player();
	void setGridPositionFrom2i(sf::Vector2i gridPosition);
	void setGridPosition(hx::Vector2d gridPosition);
	hx::Vector2d getGridPosition();
	void setSpeed(double speed);
	double getSpeed();
	void setCarryingSpeedMultiplier(double multiplier);
	double getCarryingSpeedMultiplier();
	void moveForward(double dt);
	hx::Vector2d projectForward(double dt);
	void setDirection(unsigned int direction);
	unsigned int getDirection();
	unsigned int getDirectionInDegrees();
	void setCarrying(bool isCarrying = true);
	//bool isCarrying();

private:
	sf::VertexArray m_vertices;
	//sf::ConvexShape m_convexShape;
	hx::Vector2d m_gridPosition;
	unsigned int m_direction; // 8 directions in steps of 45 degrees (clockwise from up: 0-7). this value is the direction in degrees divided by 45
	double m_speed;
	double m_carryingSpeedMultiplier;
	bool m_isCarrying;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // BRINGITBACK_PLAYER_HPP