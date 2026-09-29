#include "game_system.hpp"
#include "game_parameters.hpp"
#include "ship.hpp"
#include <iostream>

using param = Parameters;

// Define static members
sf::Texture GameSystem::spritesheet;
std::vector<std::shared_ptr<Ship>> GameSystem::ships;

void GameSystem::init() {
    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png")) {
        std::cerr << "Failed to load spritesheet from res/img/invaders_sheet.png!" << std::endl;
    }

    // 1. Player is always index 0 so Bullet knows who is who
    ships.push_back(std::make_shared<Player>());

    // 2. Spawn invader fleet grid
    for (int r = 0; r < param::rows; ++r) {
        sf::IntRect sprite_rect(
            sf::Vector2i(0, r * param::sprite_size),
            sf::Vector2i(param::sprite_size, param::sprite_size)
        );

        for (int c = 0; c < param::columns; ++c) {
            sf::Vector2f spawn_pos(
                80.0f + (c * (param::sprite_size + 14.0f)),
                50.0f + (r * (param::sprite_size + 12.0f))
            );
            ships.push_back(std::make_shared<Invader>(sprite_rect, spawn_pos));
        }
    }
}

void GameSystem::update(const float &dt) {
    for (std::shared_ptr<Ship> &s : ships) {
        s->update(dt);
    }
}

void GameSystem::render(sf::RenderWindow &window) {
    for (const std::shared_ptr<Ship> &s : ships) {
        window.draw(*s);
    }
}

void GameSystem::clean() {
    for (std::shared_ptr<Ship> &ship : ships) {
        ship.reset();
    }
    ships.clear();
}