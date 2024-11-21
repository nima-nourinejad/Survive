#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "Game.h"
#include <memory>
#include <iostream>
#include "InputHandler.h" // We included this to hande pressing escape
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>

#include "ResourceManager.h"

#include "Client.hpp"


std::string readFile (std::string const & path)
{
	std::ifstream file (path.c_str ());
	if (!file.is_open ())
		return "";
	std::stringstream read;
	read << file.rdbuf ();
	file.close ();
	return read.str ();
}

int main(int argc, char* argv[])
{
    // ResourceManager Must be Instantiated here -- DO NOT CHANGE
    ResourceManager::init(argv[0]);

    // Try to connect to the server in order to get the map
	std::cout << std::endl << "survive_client" << " is connecting to server" << std::endl;
	Client client (9001, "survive_client");
	client.customSignal ();
	while (client.signal_status != SIGINT && client.isConnected () == false)
	{
		client.connectToSocket ();
	}

	/// We close the socket after we get the map and store it in the root directory
	client.closeSocket ();
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Survive");
    window.setKeyRepeatEnabled(false);
    std::unique_ptr<Game> pGame = std::make_unique<Game>();
    if (!pGame->initialise())
    {
        std::cerr << "Game Failed to initialise" << std::endl;
        return 1;
    }
    
	//For the start screen
	pGame->setState(Game::State::WAITING);
	sf::Font font;
	font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf"));
	sf::Text text;
	text.setFont(font);
	text.setString("Start Game: Enter\nExit Game: Escape\n\nEnter your name: \n");
	text.setCharacterSize(48);
	text.setPosition(0, 100);
	sf::Text name;
	name.setFont(font);
	name.setCharacterSize(48);
	name.setPosition(0, 350);
	std::string playerName;

	sf::Text pastScores;
	pastScores.setFont(font);
	pastScores.setCharacterSize(48);
	pastScores.setPosition(0, 500);

    sf::Clock clock;

	std::chrono::time_point<std::chrono::system_clock> timePoint = std::chrono::system_clock::now ();
	std::time_t timeInSeconds = std::chrono::system_clock::to_time_t (timePoint);
	std::ofstream logFile ("score.log", std::ios::trunc);
	if (!logFile.is_open ())
		throw std::runtime_error ("Failed to open score file");
	logFile << "Past scores - " << std::put_time (std::localtime (&timeInSeconds), "%Y-%m-%d") << std::endl;
	logFile.close ();
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop

		// If the game is waiting, we only need to check for the enter key to start the game
		if (pGame->getState() == Game::State::WAITING)
		{
			if (pGame->getScore() > 0 && pGame->getGamePlayed() == true)
			{
				std::ofstream logFile ("score.log", std::ios::app);
				if (!logFile.is_open ())
					throw std::runtime_error ("Failed to open score file");
				logFile << playerName << " : " << pGame->getScore() << std::endl;
				logFile.close ();
				pGame->setScore(0);
				playerName.clear();
				name.setString(playerName);
			}

			std::string past_scores = readFile ("score.log");
			pastScores.setString(past_scores);

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
                	window.close();
				if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
					window.close();
				if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
				{
					if (playerName.size() > 0)
					{
						pGame->getClock()->restart();
						clock.restart();
						pGame->setState(Game::State::ACTIVE);
					}
				}
				if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace)
				{
					if(playerName.size() > 0)
					{
						playerName = playerName.substr(0, playerName.size() - 1);
						name.setString(playerName);
					}
				}
				if(event.type == sf::Event::TextEntered)
				{
					if((event.text.unicode >= 'A' && event.text.unicode <= 'Z') || (event.text.unicode >= 'a' && event.text.unicode <= 'z') || event.text.unicode == ' ' || (event.text.unicode <= '9' && event.text.unicode >= '0'))
					{
						playerName += static_cast<char>(event.text.unicode);
						name.setString(playerName);
					}
				}
				
			}

		}
		else
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if(pGame->getGameInput()->getEscape())
				{
					window.close();
					break;
				}

				switch(event.type)
				{
					case sf::Event::Closed:
						// "close requested" event: we close the window
						window.close();
						break;
					case sf::Event::KeyPressed:
						pGame->onKeyPressed(event.key.code);
						break;
					case sf::Event::KeyReleased:
						pGame->onKeyReleased(event.key.code);
						break;
					default:
						break;
				}
			}
			
			sf::Time elapsedTime = clock.getElapsedTime();
			clock.restart();
			pGame->update(elapsedTime.asSeconds());
		}
        
        // clear the window with black color
        window.clear(sf::Color::Black);

		// If the game is waiting, we only need to draw the text
        if (pGame->getState() == Game::State::WAITING)
		{
			window.draw(name);
			window.draw(pastScores);
			window.draw(text);
		}
		else
        	window.draw(*pGame.get());
        
        // end the current frame
        window.display();
    }
    
    return 0;
}
