#ifndef BRINGITBACK_LEVEL_HPP
#define BRINGITBACK_LEVEL_HPP

#include <SFML/System/Vector2.hpp>
#include <Hx/Plinth/Random.hpp>

#include "LevelDefinition.hpp"
#include "level1Definition.hpp"
#include "level2Definition.hpp"
#include "level3Definition.hpp"
#include "level4Definition.hpp"
#include "level5Definition.hpp"

class Level
{
public:
	Level();
	void setTile(unsigned int tileNumber, int tile);
	int getTile(unsigned int tileNumber);
	void setTileAt(sf::Vector2i position, int tile);
	int getTileAt(sf::Vector2i position);
	sf::Vector2i getStartPoint();
	unsigned int getStartDirection();
	sf::Vector2u getSize();
	unsigned int getNumberOfTiles();
	void setLevel0();
	void setLevel(unsigned int levelNumber);
	unsigned int getLevel();

	void setIsInRetrieval(bool isInRetrieval = true);
	bool isInRetrieval();
	void setIsComplete(bool isComplete = true);
	bool isComplete();

	double getTimer();
	void elapseTimer(double time);
	void lose();
	bool getLost();

	void retrievedBonusItem();
	unsigned int getBonusItemsRetrieved();

	void setHardMode(bool isHardMode = true);
	bool isHardMode();

private:
	bool m_isInRetrieval;
	bool m_isComplete;
	bool m_lost;
	bool m_isHardMode;
	const sf::Vector2u m_size{ 50u, 50u };
	const unsigned int m_numberOfTiles{ m_size.x * m_size.y }; // i.e. 50 x 50
	LevelDefinition m_level;
	unsigned int m_levelNumber;
	double m_timer;
	unsigned int m_bonusItemsRetrieved;
};

#endif // BRINGITBACK_LEVEL_HPP