#ifndef BRINGITBACK_LEVELDEFINITION_HPP
#define BRINGITBACK_LEVELDEFINITION_HPP

#include <SFML/System/Vector2.hpp>

struct LevelDefinition
{
	sf::Vector2i startPoint;
	unsigned int startDirection; // (0-7)
	unsigned int timeAllowance; // for return
	unsigned int timeBonusItems[5]; // time added on for each bonus item
	short int data[2500];
};

#endif // BRINGITBACK_LEVELDEFINITION_HPP