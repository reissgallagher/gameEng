#pragma once
#include <vector>
#include <memory>

namespace sf {
class Texture;
class RenderWindow;
}

class Ship; // Forward declaration

struct GameSystem {
    static sf::Texture spritesheet;
    static std::vector<std::shared_ptr<Ship>> ships;

    static void init();
    static void clean();
    static void update(const float &dt);
    static void render(sf::RenderWindow &window);
};