#ifndef BRINGITBACK_MAIN_HPP
#define BRINGITBACK_MAIN_HPP

#include "codeflags.hpp"

//#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Vector2.hpp>

//#include <SFML/Audio.hpp>
#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>

#include <string>
#include <vector>
#include <iostream>

#ifdef DEV_MODE
#include <Hx/_DEV_SFML/All.hpp>
#endif // DEV_MODE

//#include <Hx/Plinth.hpp>
#include <Hx/Plinth/Random.hpp>
#include <Hx/Plinth/SFML/Anchor.hpp>
#include <Hx/Plinth/SFML/ExtendedColorList.hpp>
#include <Hx/Plinth/SFML/KeyMap.hpp>

//#include <Hx/Depot.hpp>
#include <Hx/Depot/SFML/ResourceManagerBasic.hpp>

//#include <Hx/Kairos.hpp>
//#include <Hx/Kairos/Fps.hpp>
#include <Hx/Kairos/Timestep.hpp>

#include "functions.hpp"
#include "Tilemap.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "Frame.hpp"

#endif // BRINGITBACK_MAIN_HPP