#include "Weapon.h"
#include "Constants.h"

Weapon::Weapon() : Rectangle(sf::Vector2f(0, 0))
{
    setPosition(sf::Vector2f(ScreenWidth * 0.5f, ScreenHeight * 0.5f));
    setOrigin(sf::Vector2f(0.0f, 0.0f));
    setColor(sf::Color::Blue);
}

void Weapon::setActive(bool isActive, int m_direction)
{
    m_isActive = isActive;
    if (isActive && (m_direction == LEFT || m_direction == RIGHT))
    {
        setSize(sf::Vector2f(WeaponWidth, WeaponHeight));
        m_timer = WeaponActiveTime;
    }
    else if (isActive && (m_direction == UP || m_direction == DOWN))
	{
		setSize(sf::Vector2f(WeaponHeight, WeaponWidth));
		m_timer = WeaponActiveTime;
	}
	else
    {
        setSize(sf::Vector2f(0.0f, 0.0f));
        m_timer = 0.0f;
    }
}

void Weapon::update(float deltaTime)
{
    if (m_isActive)
    {
        m_timer -= deltaTime;
        if (m_timer <= 0.0f)
        {
            setActive(false, LEFT);
        }
    }
}

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape graphicsRect(getSize());
    graphicsRect.setFillColor(getColor());
    graphicsRect.setPosition(getPosition());
    target.draw(graphicsRect);
}