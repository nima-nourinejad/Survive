#include "Player.h"
#include "Weapon.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include "Game.h"
#include <iostream>

Player::Player(Game* pGame) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame),
    m_pWeapon(std::make_unique<Weapon>())
{
    setOrigin(sf::Vector2f(0.0f, 0.0f));
}

bool Player::initialise()
{
    m_sprite.setTexture(*m_pGame->getPlayerTexture());
    m_sprite.setScale(3.5f, 3.5f);
    setIsDead(false);
    setPosition(ScreenWidth / 2, ScreenHeight / 2);
    m_sprite.setPosition(getPosition());
    return true;
}

void Player::move(InputData inputData, float deltaTime)
{
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    
    xSpeed -= inputData.m_movingLeft * PlayerSpeed;
    xSpeed += inputData.m_movingRight * PlayerSpeed;
    xSpeed *= deltaTime;

    ySpeed -= inputData.m_movingUp * PlayerSpeed;
    ySpeed += inputData.m_movingDown * PlayerSpeed;
    ySpeed *= deltaTime;
    
    sf::Transformable::move(sf::Vector2f(xSpeed, ySpeed));
	// Clamp the player's position to the screen.
    setPosition(std::clamp(getPosition().x, 0.0f, ((float)ScreenWidth - PlayerWidth)), std::clamp(getPosition().y, 0.0f, ((float)ScreenHeight - PlayerHeight)));

	// if ((m_direction == UP || m_direction == DOWN))
	// 	m_pWeapon->setSize(sf::Vector2f(WeaponWidth, WeaponHeight));

    if (m_pWeapon->isActive() == false)
    {
        if (inputData.m_movingLeft == true && inputData.m_movingRight == false)
            m_direction = LEFT;
        else if (inputData.m_movingLeft == false && inputData.m_movingRight == true)
            m_direction = RIGHT;
		else if (inputData.m_movingUp == true && inputData.m_movingDown == false)
			m_direction = UP;
		else if (inputData.m_movingUp == false && inputData.m_movingDown == true)
			m_direction = DOWN;
    }
}

void Player::attack()
{
    m_pWeapon->setActive(true, m_direction);
}

void Player::update(float deltaTime)
{
    sf::Vector2f weaponSize = m_pWeapon->getSize();

    m_sprite.setPosition(getPosition());
	// Set the weapon's position based on the player's direction.
	if (m_direction == LEFT)
		m_pWeapon->setPosition(sf::Vector2f(getCenter().x - weaponSize.x, getCenter().y - weaponSize.y / 2.0f));
	else if (m_direction == RIGHT)
		m_pWeapon->setPosition(sf::Vector2f(getCenter().x, getCenter().y - weaponSize.y / 2.0f));

	else if (m_direction == UP)
		m_pWeapon->setPosition(sf::Vector2f(getCenter().x - weaponSize.x / 2.0f, getCenter().y - weaponSize.y));
	else if (m_direction == DOWN)
		m_pWeapon->setPosition(sf::Vector2f(getCenter().x - weaponSize.x / 2.0f, getCenter().y));

    m_pWeapon->update(deltaTime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Rectangle::draw(target, states);
    m_pWeapon->draw(target, states);
}