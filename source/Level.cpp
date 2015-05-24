#include "Level.hpp"


Level::Level():
m_isHardMode(false)
{
	setLevel(0);
}

void Level::setTile(unsigned int tileNumber, int tile)
{
	if (tileNumber >= 0 && tileNumber < getNumberOfTiles())
		m_level.data[tileNumber] = tile;
}

void Level::setTileAt(sf::Vector2i position, int tile)
{
	if (position.x >= 0 || position.y >= 0 || position.x < static_cast<int>(m_size.x) || position.y < static_cast<int>(m_size.y))
		setTile(position.y * m_size.x + position.x, tile);
}

int Level::getTile(unsigned int tileNumber)
{
	if (tileNumber >= 0 && tileNumber < getNumberOfTiles())
		return m_level.data[tileNumber];
	else
		return -1;
}

int Level::getTileAt(sf::Vector2i position)
{
	if (position.x < 0 || position.y < 0 || position.x >= static_cast<int>(m_size.x) || position.y >= static_cast<int>(m_size.y))
		return -1;
	return getTile(position.y * m_size.x + position.x);
}

sf::Vector2i Level::getStartPoint()
{
	return m_level.startPoint;
}

unsigned int Level::getStartDirection()
{
	return m_level.startDirection;
}

sf::Vector2u Level::getSize()
{
	return m_size;
}

unsigned int Level::getNumberOfTiles()
{
	return m_numberOfTiles;
}

void Level::setLevel0()
{
	m_timer = 0.0;
	m_levelNumber = 0u;
	m_level.startPoint = { 0, 0 };
	m_level.startDirection = 0u;
	for (unsigned int t{ 0 }; t < getNumberOfTiles(); ++t)
		m_level.data[t] = -1;
	setIsInRetrieval(false);
	setIsComplete(true);
}

void Level::setLevel(unsigned int levelNumber)
{
	const unsigned int maxLevelNumber{ 5u };
	if (levelNumber > maxLevelNumber)
		return;
	m_levelNumber = levelNumber;
	m_lost = false;
	m_bonusItemsRetrieved = 0u;
	if (m_levelNumber == 0u)
	{
		setLevel0();
		return;
	}
	else if (m_levelNumber == 1u)
		m_level = level1Definition;
	else if (m_levelNumber == 2u)
		m_level = level2Definition;
	else if (m_levelNumber == 3u)
		m_level = level3Definition;
	else if (m_levelNumber == 4u)
		m_level = level4Definition;
	else if (m_levelNumber == 5u)
		m_level = level5Definition;
	else
		return;
	setIsInRetrieval(false);
	setIsComplete(false);
	m_timer = m_level.timeAllowance;
}

unsigned int Level::getLevel()
{
	return m_levelNumber;
}

void Level::setIsInRetrieval(bool isInRetrieval)
{
	m_isInRetrieval = isInRetrieval;
}

bool Level::isInRetrieval()
{
	return m_isInRetrieval;
}

void Level::setIsComplete(bool isComplete)
{
	m_isComplete = isComplete;
}

bool Level::isComplete()
{
	return m_isComplete;
}

double Level::getTimer()
{
	return m_timer;
}

void Level::elapseTimer(double time)
{
	m_timer -= time;
}

void Level::lose()
{
	m_timer = 0.0;
	m_lost = true;
	setIsComplete();
	setIsInRetrieval(false);
}

bool Level::getLost()
{
	return m_lost;
}

void Level::retrievedBonusItem()
{
	const unsigned int maxBonusItems{ 5u };
	if (m_bonusItemsRetrieved >= maxBonusItems)
		return;
	m_timer += m_level.timeBonusItems[m_bonusItemsRetrieved];
	++m_bonusItemsRetrieved;
}

unsigned int Level::getBonusItemsRetrieved()
{
	return m_bonusItemsRetrieved;
}

void Level::setHardMode(bool isHardMode)
{
	m_isHardMode = isHardMode;
}

bool Level::isHardMode()
{
	return m_isHardMode;
}
