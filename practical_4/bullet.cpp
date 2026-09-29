#include "bullet.hpp"
#include "game_parameters.hpp"
#include "game_system.hpp"
#include "ship.hpp"

using param = Parameters;
using gs = GameSystem;

// Statically allocate pool on stack / BSS
unsigned char Bullet::_bulletPointer = 0;
Bullet Bullet::_bullets[256];

Bullet::Bullet() : _mode(false) {}

void Bullet::init() {
    for (int i = 0; i < 256; ++i) {
        _bullets[i].setTexture(gs::spritesheet);
        _bullets[i].setOrigin(sf::Vector2f(param::sprite_size / 2.0f, param::sprite_size / 2.0f));
        _bullets[i].setPosition(sf::Vector2f(-100.0f, -100.0f));
    }
}

void Bullet::fire(const sf::Vector2f &pos, const bool mode) {
    Bullet &b = _bullets[++_bulletPointer];
    b._mode = mode;
    b.setPosition(pos);

    if (mode) {
        // Invader bullet sprite
        b.setTextureRect(sf::IntRect(sf::Vector2i(64, 32), sf::Vector2i(param::sprite_size, param::sprite_size)));
    } else {
        // Player bullet sprite
        b.setTextureRect(sf::IntRect(sf::Vector2i(32, 32), sf::Vector2i(param::sprite_size, param::sprite_size)));
    }
}

void Bullet::update(const float &dt) {
    for (int i = 0; i < 256; ++i) {
        _bullets[i]._update(dt);
    }
}

void Bullet::render(sf::RenderWindow &window) {
    for (int i = 0; i < 256; ++i) {
        // Draw only bullets active on screen
        if (_bullets[i].getPosition().y > -param::sprite_size && 
            _bullets[i].getPosition().y < param::game_height + param::sprite_size) {
            window.draw(_bullets[i]);
        }
    }
}

void Bullet::_update(const float &dt) {
    // If inactive / off-screen, skip
    if (getPosition().y < -param::sprite_size || getPosition().y > param::game_height + param::sprite_size) {
        return;
    }

    // Direction: Player bullet (-y), Invader bullet (+y)
    move(sf::Vector2f(0.0f, dt * param::bullet_speed * (_mode ? 1.0f : -1.0f)));

    const sf::FloatRect boundingBox = getGlobalBounds();
    auto &player = gs::ships[0];

    for (std::shared_ptr<Ship> &s : gs::ships) {
        if (!_mode && s == player) continue;  // Player bullet doesn't hit player
        if (_mode && s != player) continue;   // Enemy bullet doesn't hit enemies

        if (!s->is_exploded() && s->getGlobalBounds().intersects(boundingBox)) {
            s->explode();
            setPosition(sf::Vector2f(-100.0f, -100.0f)); // Recycle bullet
            return;
        }
    }
}