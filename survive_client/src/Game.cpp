#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "ResourceManager.h"
#include "InputHandler.h"
#include "Weapon.h"
#include "Player.h"
#include "Rectangle.h"
#include "Vampire.h"
#include <fstream> // For reading the map file.
#include <sstream> // For reading the map file.

Game::Game() :
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_pPlayer(std::make_unique<Player>(this)),
	m_score(0), m_enemyspawned(false),
	m_gamePlayed(false)
{
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
}

Game::~Game()
{
}

bool Game::initialise()
{
    /*
        PAY ATTENTION HIVER!
            If you want to load any assets (fonts, textures) please use the pattern shown below
    */

    if (!m_font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf")))
    {
        std::cerr << "Unable to load font" << std::endl;
        return false;
    }
    if (!m_vampTexture.loadFromFile(ResourceManager::getFilePath("vampire.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_playerTexture.loadFromFile(ResourceManager::getFilePath("player.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }

    resetLevel();
    return true;
}

void Game::resetLevel()
{
    m_pVampires.clear();

    m_pPlayer->initialise();
	// Reset the player's weapon.
	m_pPlayer->getWeapon()->setActive(false, LEFT);
	std::string score = std::to_string((int)m_pClock->getElapsedTime().asSeconds());
	m_score = std::stoi(score) + 1000;
    m_pClock->restart();
	// Reset the input data.
	m_pGameInput->resetInputData();
	// Reset the game state.
	m_enemyspawned = false;
	m_pVampires.clear();
	m_pVampires.resize(0);
	
}

void Game::update(float deltaTime)
{
	// If the game is waiting, we don't need to update the game.
	m_state = State::ACTIVE;
	m_gamePlayed = true;
	if (!m_enemyspawned)
		vampireSpawner();
	m_pGameInput->update(deltaTime);
	m_pPlayer->update(deltaTime);

	for (auto& temp : m_pVampires)
	{
		temp->update(deltaTime);
	}

	if (m_pPlayer->isDead())
	{
		m_state = State::WAITING;
		resetLevel();
	}

    int i = 0;
    while (i < m_pVampires.size())
    {
        if (m_pVampires[i]->isKilled())
        {
            std::swap(m_pVampires[i], m_pVampires.back());
            m_pVampires.pop_back();
            continue;
        }
        i++;
    }
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //  Draw texts.
	sf::Text timerText;
	timerText.setFont(m_font);
	timerText.setFillColor(sf::Color::White);
	timerText.setStyle(sf::Text::Bold);
	timerText.setString(std::to_string((int)m_pClock->getElapsedTime().asSeconds()));
	timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, 20));
	target.draw(timerText);

    // Draw player.
    m_pPlayer->draw(target, states);

    //  Draw world.
    for (auto& temp : m_pVampires)
    {
        temp->draw(target, states);
    }
}


void Game::onKeyPressed(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyPressed(key);
}

void Game::onKeyReleased(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyReleased(key);
}

Player* Game::getPlayer() const 
{
    return m_pPlayer.get();
}

std::string Game::readmap(std::string const & path)
{
	std::ifstream file (path.c_str ());
	if (!file.is_open ())
		throw std::runtime_error ("Failed to open file");
	std::stringstream read;
	read << file.rdbuf ();
	file.close ();
	return read.str ();
}

std::vector<std::pair<int, int>> Game::createEnemies()
{
	std::string read = readmap ("map");
	int x = 0;
	int y = 0;
	int index = 0;
	std::vector<std::pair<int, int>> enemies;
	while (index < read.size())
	{
		if (read[index] == '\n')
		{
			x = 0;
			y++;
		}
		else
		{
			if (read[index] == 'E')
				enemies.push_back (std::make_pair (10 * x, 10 * y));
			x++;
		}
		index++;
	}
	return enemies;
}

void Game::vampireSpawner()
{
	if (m_enemyspawned)
		return;
	m_pVampires.clear();
	m_pVampires.resize(0);
    std::vector<std::pair<int, int>> enemies = createEnemies ();
	for (int i = 0; i < enemies.size(); i++)
	{
		sf::Vector2f spawnPosition = sf::Vector2f(enemies[i].first, enemies[i].second);
		m_pVampires.push_back(std::make_unique<Vampire>(this, spawnPosition));
	}
	m_enemyspawned = true;
}
