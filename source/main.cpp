// "BRING IT BACK"
// by Hapax

// for the SFML Game Jam 4
// theme: "from the end to the beginning"
// May 2015

#include "main.hpp"

#include "ResourcePath.hpp"

int main()
{
#ifdef DEV_KEEPCONSOLEOPEN
	DEV::KeepConsoleOpenSFML keepConsoleOpen;
#endif // DEV_KEEPCONSOLEOPEN

	const float volume{ 100.f };

	bool cheatShowRoute{ false };
	bool cheatSpeedBoost{ false };

	bool showInformationPage{ false };
	bool informationPage1{ true };
	bool showFullscreen{ false };
	bool mute{ false };

	const std::vector<std::string> musicFilenames
	{
		resourcePath() + "audio/forwards.ogg",
		resourcePath() + "audio/backwards.ogg"
	};
	hx::Depot::Sfml::ResourceManagerBasic resources;
	try
	{
		//resources.addFont("generic", "resources/fonts/arial.ttf");
		resources.addFont("main", resourcePath() + "fonts/msyi.ttf");
		resources.addTexture("title screen", resourcePath() + "images/titlescreen.jpg");
		resources.addTexture("title header", resourcePath() + "images/titleheader.jpg");
		resources.addTexture("title small", resourcePath() + "images/titlesmall.png");
		resources.addSoundBuffer("activate", resourcePath() + "audio/activate.wav");
	}
	catch (char* e)
	{
		std::cerr << "Failed to load resources.\nError: " << e << std::endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "Failed to load resources.\nUnknown error." << std::endl;
		return EXIT_FAILURE;
	}

	const std::string windowTitle{ "Bring It Back (by Hapax for SFML Game Jam 4 - May 2015)" };
	const sf::VideoMode defaultVideoMode{ 960, 540 };
	const sf::VideoMode fullscreenVideoMode{ sf::VideoMode::getDesktopMode() };
	//const unsigned int framerateLimit{ 60u };
	sf::RenderWindow window(defaultVideoMode, windowTitle, sf::Style::Default);
	//window.setFramerateLimit(framerateLimit);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
	sf::View view{ window.getDefaultView() };

	hx::Sfml::KeyMap keyMap;
	bool useWasdInsteadOfArrows{ true };
	setMainKeyControls(keyMap, useWasdInsteadOfArrows);
	keyMap.addControl("quit", sf::Keyboard::Escape);
	keyMap.addControl("start/continue", sf::Keyboard::Space);
	keyMap.addControl("show information page", sf::Keyboard::F1);
	keyMap.addControl("toggle fullscreen", sf::Keyboard::F2);
	keyMap.addControl("toggle main controls", sf::Keyboard::F3);
	keyMap.addControl("toggle hard mode", sf::Keyboard::F4);
	keyMap.addControl("toggle mute", sf::Keyboard::F5);
#ifdef DEV_MODE
	keyMap.addControl("reset player position", sf::Keyboard::F12);
	keyMap.addControl("reset current level", sf::Keyboard::BackSpace);
	keyMap.addControl("reset to level 0", sf::Keyboard::Num0);
	keyMap.addControl("reset to level 1", sf::Keyboard::Num1);
	keyMap.addControl("reset to level 2", sf::Keyboard::Num2);
	keyMap.addControl("reset to level 3", sf::Keyboard::Num3);
	keyMap.addControl("reset to level 4", sf::Keyboard::Num4);
	keyMap.addControl("reset to level 5", sf::Keyboard::Num5);
#ifdef DEV_ALLOWCHEATS
	keyMap.addControl("cheat speed boost", sf::Keyboard::Add);
	keyMap.addControl("cheat show route", sf::Keyboard::Subtract);
#endif // DEV_ALLOWCHEATS
#endif // DEV_MODE

	hx::Random random;

	Level level;
	
	Tilemap tilemap;
	const sf::Vector2u mapSize{ (tilemap.gridSize.x - 1) * tilemap.tileSize.x, (tilemap.gridSize.y - 1) * tilemap.tileSize.y };
	const sf::Vector2u mapPosition{ (static_cast<unsigned int>(view.getSize().x) - mapSize.x) / 2u, (static_cast<unsigned int>(view.getSize().y) - mapSize.y) / 2u };
	tilemap.setPosition(sf::Vector2f(mapPosition));

	Frame tilemapFrame;
	tilemapFrame.setInnerRect(sf::FloatRect(static_cast<float>(mapPosition.x), static_cast<float>(mapPosition.y), static_cast<float>(mapSize.x), static_cast<float>(mapSize.y)));
	tilemapFrame.setOuterRect(sf::FloatRect(0.f, 0.f, static_cast<float>(view.getSize().x), static_cast<float>(view.getSize().y)));
	tilemapFrame.setColor(sf::Colors::DarkGrey);

	Player player;
	player.setPosition(sf::Vector2f(mapPosition) + sf::Vector2f(mapSize / 2u));
	player.setGridPositionFrom2i(level.getStartPoint());

	sf::Text legendText;
	legendText.setFont(resources.getFont("main"));
	legendText.setString("\
BLACK = outside\n\
BROWN = floor\n\
RED = wall\n\
DARK RED = door (unmarked)\n\
BLUE = open door (the route)\n\
GREEN = open door\n\
CYAN = locked door\n\
YELLOW = objective\n\
ORANGE = bonus item\
");
	legendText.setCharacterSize(16);
	const sf::Vector2f legendTextLocalBottomRight{ sf::Anchor::Local::getBottomRight(legendText) };
	legendText.setPosition(sf::Vector2f(view.getSize()) - legendTextLocalBottomRight - sf::Vector2f(8.f, 8.f));

	sf::Sprite titleScreen(resources.getTexture("title screen"));
	sf::Sprite titleHeader(resources.getTexture("title header"));
	sf::Sprite titleSmall(resources.getTexture("title small"));

	sf::Text pressSpaceToPlayText, pressF1ForInformationText;
	pressSpaceToPlayText.setFont(resources.getFont("main"));
	pressSpaceToPlayText.setStyle(sf::Text::Style::Bold);
	pressSpaceToPlayText.setCharacterSize(60);
	pressSpaceToPlayText.setString("PRESS SPACE TO PLAY");
	pressSpaceToPlayText.setPosition(positionToCenterTextInWindow(pressSpaceToPlayText, view).x, 370.f);
	const std::string pressF1ForInformationString{ "PRESS F1 FOR INFORMATION" };
	const std::string pressF1ForInformationShowingString{ "ESCAPE: BACK TO TITLE SCREEN      F1: MORE INFORMATION" };
	pressF1ForInformationText.setFont(resources.getFont("main"));
	pressF1ForInformationText.setStyle(sf::Text::Style::Regular);
	pressF1ForInformationText.setColor(sf::Colors::Yellow);
	pressF1ForInformationText.setCharacterSize(32);
	//pressF1ForInformationText.setString(pressF1ForInformationString);


	sf::Text levelCompleteText, pressSpaceToContinueText;
	const std::string levelCompleteString{ "LEVEL COMPLETE" };
	const std::string hardModeCompleteString{ "HARD MODE COMPLETE\n  CONGRATULATIONS!" };
	const std::string gameCompleteString{ "NORMAL MODE COMPLETE\n   CONGRATULATIONS!" };
	const std::string levelFailedString{ "LEVEL FAILED" };
	levelCompleteText.setFont(resources.getFont("main"));
	const std::string pressSpaceToContinueString{ "PRESS SPACE TO CONTINUE" };
	const std::string pressSpaceToRetryLevelString{ "PRESS SPACE TO RETRY LEVEL" };
	const std::string pressSpaceToReturnToTitleScreenString{ "PRESS SPACE TO END GAME" };
	pressSpaceToContinueText.setFont(resources.getFont("main"));
	pressSpaceToContinueText.setCharacterSize(30);
	pressSpaceToContinueText.setColor(sf::Colors::Yellow);
	//pressSpaceToContinueText.setString(pressSpaceToContinueString);

	sf::Text toggleHardModeText;
	toggleHardModeText.setFont(resources.getFont("main"));
	toggleHardModeText.setCharacterSize(16);
	const sf::Color hardModeOffColor{ sf::Colors::Green };
	const sf::Color hardModeOnColor{ sf::Colors::Red };
	const std::string hardModeOnString{ "(F4) Difficulty level: HARD" };
	const std::string hardModeOffString{ "(F4) Difficulty level: normal" };

	sf::Text toggleMuteText;
	toggleMuteText.setFont(resources.getFont("main"));
	toggleMuteText.setCharacterSize(16);
	const std::string soundOnString{ "(F5) Sound: on" };
	const std::string soundOffString{ "(F5) Sound: off" };
	toggleMuteText.setColor(sf::Colors::LightGrey);
	toggleMuteText.setString(soundOffString);
	const sf::Vector2f toggleMuteTextLocalTopRight{ sf::Anchor::Local::getTopRight(toggleMuteText) };
	toggleMuteText.setPosition(view.getSize().x - toggleMuteTextLocalTopRight.x - 2.f, 2.f - toggleMuteTextLocalTopRight.y);

	sf::Text informationPage;
	informationPage.setFont(resources.getFont("main"));
	informationPage.setCharacterSize(18);
	std::string informationPage1String{ "\
Go to the objective (yellow), following the route highlighted by the blue doorways.\n\
Some doors that are not a part of the route are open too (green doorways) so be careful not to get lost!\n\
When you get to the end of the route and collect the object, you must travel back to the beginning again.\n\
After you go from the end to the beginning before the timer expires, you complete the level.\n\
However, the route is no longer highlighted so you must remember the route!\n\
During the return, all doors (open, closed, part of the route or not) look alike!\n\
There are bonus items (orange) to collect too.\n\
\n\
Walls (red) and locked doors (cyan) stop your motion.\n\
When you reach the objective and initiate the retrieval journey, your speed is dramatically increased.\n\
The bonus items can only be collected after the objective has been reached.\n\
\n\
There are five levels of increasing difficulty.\n\
Can you complete all five before time runs out?\n\
Are you able to collect all of the bonus items?\n\
Playing on hard difficulty requires ALL of the bonus items to be collected.\n\
Good luck!\n\
" };
	std::string informationPage2String{ "\
CONTROLS:\n\
Use the WASD/arrow keys to move.\n\
The Escape key goes back: quits current game, closes this information, and closes game when at title screen.\n\
F2 will toggle fullscreen mode (uses current desktop resolution)\n\
F3 will toggle between WASD and arrow keys.\n\
\n\
This game was created entirely from scratch by Hapax (github.com/hapaxia)\n\
- including all levels, graphics, sound, and music - \n\
in under 72 hours for the SFML Game Jam 4 - May 2015\n\
using the SFML library (www.sfml-dev.org)\n\
and Hx library (github.com/hapaxia/hx)\n\
" };
	
	sf::Text countdownTimer;
	countdownTimer.setFont(resources.getFont("main"));
	countdownTimer.setColor(sf::Colors::Red);
	countdownTimer.setCharacterSize(80);
	countdownTimer.setStyle(sf::Text::Style::Bold);
	countdownTimer.setPosition(static_cast<float>(mapPosition.x + mapSize.x + 10u), 200);

	sf::Text bonusItemsRemainingText;
	bonusItemsRemainingText.setFont(resources.getFont("main"));
	bonusItemsRemainingText.setColor(sf::Colors::White);
	bonusItemsRemainingText.setCharacterSize(30);
	bonusItemsRemainingText.setPosition(10, 250);

	sf::Listener::setGlobalVolume(volume);
	sf::Sound activateSound(resources.getSoundBuffer("activate"));
	sf::Music music;

	//hx::Kairos::Fps fps;
	hx::Kairos::Timestep timestep;
	timestep.setStep(0.01); // 100 FPS
	timestep.setMaxAccumulation(0.1);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == keyMap.getKey("quit") && !showInformationPage && level.getLevel() == 0u)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == keyMap.getKey("start/continue") && !showInformationPage && level.isComplete())
				{
					if (level.getLost())
						resetToLevel(level.getLevel(), player, level, music, musicFilenames);
					else if (level.getLevel() == 5u)
						resetToLevel(0, player, level, music, musicFilenames);
					else
						resetToLevel(level.getLevel() + 1u, player, level, music, musicFilenames);
				}
				else if (event.key.code == keyMap.getKey("quit"))
				{
					if (level.getLevel() != 0u)
						resetToLevel(0, player, level, music, musicFilenames);
					showInformationPage = false;
				}
				else if (event.key.code == keyMap.getKey("quit") && level.getLevel() != 0u)
					resetToLevel(0, player, level, music, musicFilenames);
				else if (event.key.code == keyMap.getKey("show information page") && level.getLevel() == 0u)
				{
					if (showInformationPage)
						informationPage1 = !informationPage1;
					else
						informationPage1 = true;
					showInformationPage = true;
				}
				else if (event.key.code == keyMap.getKey("toggle fullscreen"))
				{
					showFullscreen = !showFullscreen;
					if (showFullscreen)
						window.create(fullscreenVideoMode, windowTitle, sf::Style::Fullscreen);
					else
						window.create(defaultVideoMode, windowTitle, sf::Style::Default);
					//window.setFramerateLimit(framerateLimit);
					window.setVerticalSyncEnabled(true);
					window.setMouseCursorVisible(false);
					window.setView(view);
				}
				else if (event.key.code == keyMap.getKey("toggle main controls"))
				{
					useWasdInsteadOfArrows = !useWasdInsteadOfArrows;
					setMainKeyControls(keyMap, useWasdInsteadOfArrows);
				}
				else if (event.key.code == keyMap.getKey("toggle mute"))
				{
					mute = !mute;
					if (mute)
						sf::Listener::setGlobalVolume(0.f);
					else
						sf::Listener::setGlobalVolume(volume);
				}
				else if (event.key.code == keyMap.getKey("toggle hard mode") && level.getLevel() == 0u)
					level.setHardMode(!level.isHardMode());
#ifdef DEV_MODE
				else if (event.key.code == keyMap.getKey("reset player position"))
					player.setGridPositionFrom2i(level.getStartPoint());
				else if (event.key.code == keyMap.getKey("reset current level"))
					resetToLevel(level.getLevel(), player, level);
				else if (event.key.code == keyMap.getKey("reset to level 0"))
					resetToLevel(0, player, level);
				else if (event.key.code == keyMap.getKey("reset to level 1"))
					resetToLevel(1, player, level);
				else if (event.key.code == keyMap.getKey("reset to level 2"))
					resetToLevel(2, player, level);
				else if (event.key.code == keyMap.getKey("reset to level 3"))
					resetToLevel(3, player, level);
				else if (event.key.code == keyMap.getKey("reset to level 4"))
					resetToLevel(4, player, level);
				else if (event.key.code == keyMap.getKey("reset to level 5"))
					resetToLevel(5, player, level);
#endif // DEV_MODE
			}
		}

#ifdef DEV_ALLOWCHEATS
		if (sf::Keyboard::isKeyPressed(keyMap.getKey("cheat show route")))
			cheatShowRoute = true;
		else
			cheatShowRoute = false;
		if (sf::Keyboard::isKeyPressed(keyMap.getKey("cheat speed boost")))
			cheatSpeedBoost = true;
		else
			cheatSpeedBoost = false;
#endif // DEV_ALLOWCHEATS

		
		
		if (!level.isComplete())
		{
			while (timestep.isUpdateRequired())
			{
#ifdef DEV_ALLOWCHEATS
				if (cheatSpeedBoost)
					processTimestep(timestep.getStep() * 2.0, keyMap, player, level, music, musicFilenames, activateSound);
				else
#endif // DEV_ALLOWCHEATS
					processTimestep(timestep.getStep(), keyMap, player, level, music, musicFilenames, activateSound);
			}
		}



		// frame

		//fps.update();
		timestep.addFrame();

		// update tilemap from level data
		for (unsigned int y{ 0 }; y < tilemap.gridSize.y; ++y)
		{
			for (unsigned int x{ 0 }; x < tilemap.gridSize.x; ++x)
			{
				int tile{ -1 };
				sf::Vector2i currentGridPosition{ static_cast<int>(floor(player.getGridPosition().x) + x - tilemap.gridSize.x / 2), static_cast<int>(floor(player.getGridPosition().y) + y - tilemap.gridSize.y / 2) };
				if (currentGridPosition.x >= 0 && currentGridPosition.y >= 0 && currentGridPosition.x < static_cast<int>(level.getSize().x) && currentGridPosition.y < static_cast<int>(level.getSize().y))
					tile = level.getTile(currentGridPosition.y * level.getSize().x + currentGridPosition.x);
				tilemap.setTile(y * tilemap.gridSize.x + x, tile);
			}
		}
		tilemap.setOffset({ player.getGridPosition().x - floor(player.getGridPosition().x), player.getGridPosition().y - floor(player.getGridPosition().y) });
		if (level.isInRetrieval() && !cheatShowRoute)
			tilemap.setEntering(false);
		else
			tilemap.setEntering();
		tilemap.update();

		//window.setTitle("FPS: " + std::to_string(static_cast<unsigned int>(fps.getFps())) + " | Time: " + std::to_string(timestep.getTime()) + " | Player Grid Position: " + std::to_string(player.getGridPosition().x) + ", " + std::to_string(player.getGridPosition().y));

		window.clear();
		if (level.getLevel() == 0u)
		{
			if (showInformationPage)
			{
				if (informationPage1)
					informationPage.setString(informationPage1String);
				else
					informationPage.setString(informationPage2String);
				informationPage.setPosition(floor(positionToCenterTextInWindow(informationPage, view).x), 155.f);
				window.draw(titleHeader);
				window.draw(informationPage);
				pressF1ForInformationText.setString(pressF1ForInformationShowingString);
			}
			else
			{
				window.draw(titleScreen);
				window.draw(pressSpaceToPlayText);
				pressF1ForInformationText.setString(pressF1ForInformationString);
			}
			pressF1ForInformationText.setPosition(positionToCenterTextInWindow(pressF1ForInformationText, view).x, 480);
			window.draw(pressF1ForInformationText);

			if (level.isHardMode())
			{
				toggleHardModeText.setColor(hardModeOnColor);
				toggleHardModeText.setString(hardModeOnString);
			}
			else
			{
				toggleHardModeText.setColor(hardModeOffColor);
				toggleHardModeText.setString(hardModeOffString);
			}
			const sf::Vector2f toggleHardModeTextLocalBottomRight{ sf::Anchor::Local::getBottomRight(toggleHardModeText) };
			toggleHardModeText.setPosition(4.f, view.getSize().y - toggleHardModeTextLocalBottomRight.y - 4.f);
			window.draw(toggleHardModeText);
		}
		else
		{
			if (level.isComplete())
			{
				if (level.getLost())
				{
					levelCompleteText.setCharacterSize(60);
					levelCompleteText.setString(levelFailedString);
					pressSpaceToContinueText.setString(pressSpaceToRetryLevelString);
				}
				else if (level.getLevel() == 5u) // number of final level (5)
				{
					if (level.isHardMode())
					{
						levelCompleteText.setCharacterSize(50);
						levelCompleteText.setString(hardModeCompleteString);
					}
					else
					{
						levelCompleteText.setCharacterSize(45);
						levelCompleteText.setString(gameCompleteString);
					}
					pressSpaceToContinueText.setString(pressSpaceToReturnToTitleScreenString);
				}
				else
				{
					levelCompleteText.setCharacterSize(60);
					levelCompleteText.setString(levelCompleteString);
					pressSpaceToContinueText.setString(pressSpaceToContinueString);
				}
				levelCompleteText.setPosition(positionToCenterTextInWindow(levelCompleteText, view).x, 175.f);
				window.draw(levelCompleteText);
				pressSpaceToContinueText.setPosition(positionToCenterTextInWindow(pressSpaceToContinueText, view).x, 375.f);
				window.draw(pressSpaceToContinueText);
			}
			else
			{
				window.draw(tilemap);
				window.draw(player);
			}
			window.draw(tilemapFrame);
			window.draw(titleSmall);
			window.draw(legendText);
			if (level.isInRetrieval())
			{
				countdownTimer.setString(std::to_string(static_cast<unsigned int>(ceil(level.getTimer()))));
				window.draw(countdownTimer);
				bonusItemsRemainingText.setString("Items remaining: " + std::to_string(level.getLevel() - level.getBonusItemsRetrieved()));
				window.draw(bonusItemsRemainingText);
			}
		}
		if (mute)
			toggleMuteText.setString(soundOffString);
		else
			toggleMuteText.setString(soundOnString);
		window.draw(toggleMuteText);
		window.display();
	}

#ifdef DEV_KEEPCONSOLEOPEN
	keepConsoleOpen.allowToClose();
#endif // DEV_KEEPCONSOLEOPEN

	return EXIT_SUCCESS;
}