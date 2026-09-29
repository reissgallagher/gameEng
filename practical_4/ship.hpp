#pragma once
#include <SFML/Graphics.hpp>

class Ship : public sf::Sprite {
public:
    Ship();
    Ship(const Ship &s);
    Ship(sf::IntRect ir);
    virtual ~Ship() = 0; // Pure virtual destructor makes Ship abstract

    virtual void update(const float &dt);
    virtual void move_down() {} // Invaders override; Player ignores

    bool is_exploded() const { return _exploded; }
    virtual void explode();

protected:
    sf::IntRect _sprite;
    bool _exploded = false;
};

class Invader : public Ship {
public:
    static bool direction; // true = right, false = left
    static float speed;
    static float acc;

    Invader();
    Invader(const Invader &inv);
    Invader(sf::IntRect ir, sf::Vector2f pos);

    void update(const float &dt) override;
    void move_down() override;

private:
    float _explosion_timer = 0.5f;
    float _fire_timer = 0.0f;
};

class Player : public Ship {
public:
    Player();
    void update(const float &dt) override;

private:
    float _fire_cooldown = 0.0f;
};