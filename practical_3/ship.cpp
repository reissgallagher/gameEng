#include "ship.hpp"
#include "game_system.hpp"
#include "game_parameters.hpp"
#include "bullet.hpp"
#include <cstdlib>

using param = Parameters;
using gs = GameSystem;

// Initialize static Invader synchronization variables
bool Invader::direction = true;
float Invader::speed = param::invader_start_speed;
float Invader::acc = param::invader_acc;

// --- Base Ship Implementation ---
Ship::Ship() : _exploded(false) {}

Ship::Ship(const Ship &s) : sf::Sprite(s), _sprite(s._sprite), _exploded(s._exploded) {}

Ship::Ship(sf::IntRect ir) : sf::Sprite(), _sprite(ir), _exploded(false) {
    setTexture(GameSystem::spritesheet);
    setTextureRect(_sprite);
}

Ship::~Ship() = default;

void Ship::update(const float &dt) {}

void Ship::explode() {
    // Switch to explosion sprite
    setTextureRect(sf::IntRect(sf::Vector2i(128, 32), sf::Vector2i(param::sprite_size, param::sprite_size)));
    _exploded = true;
}

// --- Invader Implementation ---
Invader::Invader() : Ship() {}

Invader::Invader(const Invader &inv) : Ship(inv) {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
    setOrigin(sf::Vector2f(param::sprite_size / 2.0f, param::sprite_size / 2.0f));
    setPosition(pos);
    _fire_timer = 2.0f + static_cast<float>(rand() % 50) / 10.0f;
}

void Invader::move_down() {
    move(sf::Vector2f(0.0f, 24.0f));
}

void Invader::update(const float &dt) {
    Ship::update(dt);

    if (_exploded) {
        _explosion_timer -= dt;
        if (_explosion_timer <= 0.0f) {
            // Despawn off-screen once explosion animation finishes
            setPosition(sf::Vector2f(-200.0f, -200.0f));
        }
        return;
    }

    // Horizontal march
    move(sf::Vector2f(dt * (direction ? 1.0f : -1.0f) * speed, 0.0f));

    // Fleet bounce & drop synchronization
    if ((direction && getPosition().x > param::game_width - (param::sprite_size / 2.0f)) ||
        (!direction && getPosition().x < (param::sprite_size / 2.0f))) {
        
        direction = !direction;
        speed += acc;

        for (std::shared_ptr<Ship> &ship : gs::ships) {
            ship->move_down();
        }
    }

    // Invader firing cooldown
    _fire_timer -= dt;
    if (_fire_timer <= 0.0f && (rand() % 100 == 0)) {
        Bullet::fire(getPosition(), true);
        _fire_timer = 4.0f + static_cast<float>(rand() % 60) / 10.0f;
    }
}

// --- Player Implementation ---
Player::Player() : Ship(sf::IntRect(
    sf::Vector2i(param::sprite_size * 5, param::sprite_size),
    sf::Vector2i(param::sprite_size, param::sprite_size)
)) {
    setOrigin(sf::Vector2f(param::sprite_size / 2.0f, param::sprite_size / 2.0f));
    setPosition(sf::Vector2f(param::game_width / 2.0f, param::game_height - static_cast<float>(param::sprite_size)));
}

void Player::update(const float &dt) {
    Ship::update(dt);

    if (_exploded) return;

    // Movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if (getPosition().x > param::sprite_size / 2.0f) {
            move(sf::Vector2f(-param::player_speed * dt, 0.0f));
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (getPosition().x < param::game_width - (param::sprite_size / 2.0f)) {
            move(sf::Vector2f(param::player_speed * dt, 0.0f));
        }
    }

    // Shooting with weapon cooldown
    _fire_cooldown -= dt;
    if (_fire_cooldown <= 0.0f && (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))) {
        Bullet::fire(getPosition(), false);
        _fire_cooldown = 0.5f;
    }
}