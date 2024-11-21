#pragma once

#include "Rectangle.h"

class Weapon : public Rectangle
{
public:
    Weapon();
    virtual ~Weapon() {}

    void setActive(bool isActive, int m_direction);
    void update(float deltaTime);
    bool isActive() { return m_isActive; }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    bool m_isActive = false;
    float m_timer = 0.0f;
};