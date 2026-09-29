#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite {
public:
    static void init();
    static void update(const float &dt);
    static void render(sf::RenderWindow &window);
    static void fire(const sf::Vector2f &pos, const bool mode); // false = Player, true = Invader

    Bullet();
    ~Bullet() = default;

private:
    void _update(const float &dt);

    bool _mode; // false = Player bullet (up), true = Enemy bullet (down)
    static unsigned char _bulletPointer;
    static Bullet _bullets[256];
};