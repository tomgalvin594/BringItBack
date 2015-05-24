#ifndef BRINGITBACK_FUNCTIONS_HPP
#define BRINGITBACK_FUNCTIONS_HPP


#include <Hx/Kairos/Timestep.hpp>
#include <Hx/Plinth/SFML/KeyMap.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <iostream>

#include "codeflags.hpp"
#include "Player.hpp"
#include "Level.hpp"

bool isTileBlocking(int tile);
void processTimestep(double dt, hx::Sfml::KeyMap& keyMap, Player& player, Level& level, sf::Music& music, const std::vector<std::string>& musicFilenames, sf::Sound& sound);
void resetToLevel(unsigned int levelNumber, Player& player, Level& level, sf::Music& music, const std::vector<std::string>& musicFilenames);
sf::Vector2f positionToCenterTextInWindow(sf::Text& text, sf::View& view);
void setMainKeyControls(hx::Sfml::KeyMap& keyMap, bool useWasd = true);

#endif // BRINGITBACK_FUNCTIONS_HPP