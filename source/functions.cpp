#include "functions.hpp"

bool isTileBlocking(int tile)
{
	switch (tile)
	{
	case 1: // red - main wall
	case 4: // cyan - unusable door
		return true;
	default:
		return false;
	}
}

void processTimestep(double dt, hx::Sfml::KeyMap& keyMap, Player& player, Level& level, sf::Music& music, const std::vector<std::string>& musicFilenames, sf::Sound& sound)
{
#ifndef DEV_STOPTIMER
	if (level.isInRetrieval())
		level.elapseTimer(dt);
#endif // DEV_STOPTIMER
	if (level.getTimer() < 0.0)
	{
		level.lose();
		music.stop();
		return;
	}

	sf::Vector2i playerDirectionControl{ 0, 0 };
	if (sf::Keyboard::isKeyPressed(keyMap.getKey("up")))
		--playerDirectionControl.y;
	if (sf::Keyboard::isKeyPressed(keyMap.getKey("down")))
		++playerDirectionControl.y;
	if (sf::Keyboard::isKeyPressed(keyMap.getKey("left")))
		--playerDirectionControl.x;
	if (sf::Keyboard::isKeyPressed(keyMap.getKey("right")))
		++playerDirectionControl.x;

	if (playerDirectionControl == sf::Vector2i(0, -1))
		player.setDirection(0);
	else if (playerDirectionControl == sf::Vector2i(1, -1))
		player.setDirection(1);
	else if (playerDirectionControl == sf::Vector2i(1, 0))
		player.setDirection(2);
	else if (playerDirectionControl == sf::Vector2i(1, 1))
		player.setDirection(3);
	else if (playerDirectionControl == sf::Vector2i(0, 1))
		player.setDirection(4);
	else if (playerDirectionControl == sf::Vector2i(-1, 1))
		player.setDirection(5);
	else if (playerDirectionControl == sf::Vector2i(-1, 0))
		player.setDirection(6);
	else if (playerDirectionControl == sf::Vector2i(-1, -1))
		player.setDirection(7);
	player.setRotation(static_cast<float>(player.getDirectionInDegrees()));

	hx::Vector2d projectedPlayerPosition = player.projectForward(dt);
	int left{ static_cast<int>(floor(projectedPlayerPosition.x - 0.5)) };
	int top{ static_cast<int>(floor(projectedPlayerPosition.y - 0.5)) };
	int right{ static_cast<int>(floor(projectedPlayerPosition.x + 0.5)) };
	int bottom{ static_cast<int>(floor(projectedPlayerPosition.y + 0.5)) };
	sf::Vector2i center{ static_cast<int>(floor(projectedPlayerPosition.x)), static_cast<int>(floor(projectedPlayerPosition.y)) };
	if (!isTileBlocking(level.getTileAt({ left, top })) &&
		!isTileBlocking(level.getTileAt({ center.x, top })) &&
		!isTileBlocking(level.getTileAt({ right, top })) &&
		!isTileBlocking(level.getTileAt({ right, center.y })) &&
		!isTileBlocking(level.getTileAt({ right, bottom })) &&
		!isTileBlocking(level.getTileAt({ center.x, bottom })) &&
		!isTileBlocking(level.getTileAt({ left, bottom })) &&
		!isTileBlocking(level.getTileAt({ left, center.y })) &&
		playerDirectionControl != sf::Vector2i(0, 0))
		player.moveForward(dt);

	sf::Vector2i playerCurrentGridPosition{ static_cast<int>(floor(player.getGridPosition().x)), static_cast<int>(floor(player.getGridPosition().y)) };
	if (level.getTileAt(playerCurrentGridPosition) == 5) // target
	{
		sound.play();
		if (!music.openFromFile(musicFilenames.back()))
			std::cerr << "Could not find music file." << std::endl;

		level.setTileAt(playerCurrentGridPosition, 0);
		level.setIsInRetrieval();
		player.setCarrying();
	}
	else if (level.isInRetrieval() && level.getTileAt(playerCurrentGridPosition) == 6) // bonus
	{
		level.setTileAt(playerCurrentGridPosition, 0);
		level.retrievedBonusItem();
	}

	if (level.getTileAt(playerCurrentGridPosition) == -1) // outside
	{
		if (level.isInRetrieval())
		{
			level.setIsComplete();
			if (level.isHardMode() && (level.getBonusItemsRetrieved() < level.getLevel()))
				level.lose();
			music.stop();
		}
		level.setIsInRetrieval(false);
		player.setCarrying(false);
	}

	const double mapBoundaryPadding{ 2.0 };
	if (player.getGridPosition().x < -mapBoundaryPadding)
		player.setGridPosition({ -mapBoundaryPadding, player.getGridPosition().y });
	else if (player.getGridPosition().x > level.getSize().x + mapBoundaryPadding)
		player.setGridPosition({ level.getSize().x + mapBoundaryPadding, player.getGridPosition().y });
	if (player.getGridPosition().y < -mapBoundaryPadding)
		player.setGridPosition({ player.getGridPosition().x, -mapBoundaryPadding });
	else if (player.getGridPosition().y > level.getSize().y + mapBoundaryPadding)
		player.setGridPosition({ player.getGridPosition().x, level.getSize().y + mapBoundaryPadding });

	if (level.isInRetrieval() && !level.isComplete() && sound.getStatus() != sf::Sound::Playing && music.getStatus() != sf::Music::Playing)
		music.play();
}

void resetToLevel(unsigned int levelNumber, Player& player, Level& level, sf::Music& music, const std::vector<std::string>& musicFilenames)
{
	level.setLevel(levelNumber);
	if (!music.openFromFile(musicFilenames.front()))
		std::cerr << "Could not find music file." << std::endl;
	else if (level.getLevel() != 0u)
		music.play();
	player.setGridPositionFrom2i(level.getStartPoint());
	player.setDirection(level.getStartDirection());
	player.setCarrying(false);
}

sf::Vector2f positionToCenterTextInWindow(sf::Text& text, sf::View& view)
{
	return (sf::Vector2f(view.getSize()) - sf::Vector2f(text.getLocalBounds().width + text.getLocalBounds().left, text.getLocalBounds().height + text.getLocalBounds().top)) / 2.f;
}

void setMainKeyControls(hx::Sfml::KeyMap& keyMap, bool useWasd)
{
	keyMap.removeControl("up");
	keyMap.removeControl("down");
	keyMap.removeControl("left");
	keyMap.removeControl("right");
	if (useWasd)
	{
		keyMap.addControl("up", sf::Keyboard::W);
		keyMap.addControl("down", sf::Keyboard::S);
		keyMap.addControl("left", sf::Keyboard::A);
		keyMap.addControl("right", sf::Keyboard::D);
	}
	else
	{
		keyMap.addControl("up", sf::Keyboard::Up);
		keyMap.addControl("down", sf::Keyboard::Down);
		keyMap.addControl("left", sf::Keyboard::Left);
		keyMap.addControl("right", sf::Keyboard::Right);
	}
}
