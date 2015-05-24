#include "Player.hpp"


Player::Player():
m_vertices(sf::TrianglesFan, 8),
//m_vertices(sf::Quads, 4),
//m_convexShape(),
m_gridPosition({ 0.0, 0.0 }),
m_direction(0),
m_speed(7.0),
m_carryingSpeedMultiplier(1.5),
m_isCarrying(false)
{
	const sf::Vector2f arrowSize{ 15.f, 15.f };
	///*
	m_vertices[0].position = { 0.f, -1.f * arrowSize.y };
	m_vertices[1].position = { arrowSize.x, 0.f };
	m_vertices[2].position = { arrowSize.x / 2.f, 0.f };
	m_vertices[3].position = { arrowSize.x / 2.f, 1.f * arrowSize.y };

	m_vertices[4].position = { -arrowSize.x / 2.f, 1.f * arrowSize.y };
	m_vertices[5].position = { -arrowSize.x / 2.f, 0.f };
	m_vertices[6].position = { -arrowSize.x, 0.f };
	m_vertices[7].position = { 0.f, -1.f * arrowSize.y };

	for (unsigned int v{ 0 }; v < m_vertices.getVertexCount(); ++v)
		m_vertices[v].color = sf::Colors::White;
	//*/
	/*
	m_convexShape.setPointCount(7);
	m_convexShape.setPoint(0, sf::Vector2f(0.f, -1.f * arrowSize.y));
	m_convexShape.setPoint(1, sf::Vector2f(arrowSize.x, 0.f));
	m_convexShape.setPoint(2, sf::Vector2f(arrowSize.x / 2.f, 0.f));
	m_convexShape.setPoint(3, sf::Vector2f(arrowSize.x / 2.f, 1.f * arrowSize.y));
	m_convexShape.setPoint(4, sf::Vector2f(-arrowSize.x / 2.f, 1.f * arrowSize.y));
	m_convexShape.setPoint(5, sf::Vector2f(-arrowSize.x / 2.f, 0.f));
	m_convexShape.setPoint(6, sf::Vector2f(-arrowSize.x, 0.f));
	m_convexShape.setFillColor(sf::Colors::White);
	m_convexShape.setOutlineColor(sf::Colors::Black);
	m_convexShape.setOutlineThickness(2.f);
	*/
}

void Player::setGridPositionFrom2i(sf::Vector2i gridPosition)
{
	setGridPosition(hx::Vector2d{ static_cast<double>(gridPosition.x), static_cast<double>(gridPosition.y) });
}

void Player::setGridPosition(hx::Vector2d gridPosition)
{
	m_gridPosition = gridPosition;
}

hx::Vector2d Player::getGridPosition()
{
	return m_gridPosition;
}

void Player::setSpeed(double speed)
{
	m_speed = speed;
}

double Player::getSpeed()
{
	return m_speed;
}

void Player::setCarryingSpeedMultiplier(double multiplier)
{
	m_carryingSpeedMultiplier = multiplier;
}

double Player::getCarryingSpeedMultiplier()
{
	return m_carryingSpeedMultiplier;
}

void Player::moveForward(double dt)
{
	const double angleOnAxis{ 0.7071067811865 };

	if (m_isCarrying)
		dt *= m_carryingSpeedMultiplier;

	hx::Vector2d move{ 0.0, 0.0 };
	if (m_direction == 0)
		move = { 0.0, -1.0 };
	else if (m_direction == 1)
		move = { angleOnAxis, -angleOnAxis };
	else if (m_direction == 2)
		move = { 1.0, 0.0 };
	else if (m_direction == 3)
		move = { angleOnAxis, angleOnAxis };
	else if (m_direction == 4)
		move = { 0.0, 1.0 };
	else if (m_direction == 5)
		move = { -angleOnAxis, angleOnAxis };
	else if (m_direction == 6)
		move = { -1.0, 0.0 };
	else if (m_direction == 7)
		move = { -angleOnAxis, -angleOnAxis };

	m_gridPosition.x += move.x * m_speed * dt;
	m_gridPosition.y += move.y * m_speed * dt;
}

hx::Vector2d Player::projectForward(double dt)
{
	moveForward(dt);
	hx::Vector2d position = m_gridPosition;
	moveForward(-dt);
	return position;
}

void Player::setDirection(unsigned int direction)
{
	m_direction = hx::clampCycle(direction, 0u, 7u);
}

unsigned int Player::getDirection()
{
	return m_direction;
}

unsigned int Player::getDirectionInDegrees()
{
	return m_direction * 45u;
}

void Player::setCarrying(bool isCarrying)
{
	m_isCarrying = isCarrying;
}

/*
bool Player::isCarrying()
{
	return m_isCarrying;
}
*/




// PRIVATE

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;
	target.draw(m_vertices, states);
	//target.draw(m_convexShape, states);
}