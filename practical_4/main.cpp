#include <SFML/Graphics.hpp>
#include "game_parameters.hpp"
#include "game_system.hpp"
#include "bullet.hpp"

using param = Parameters;

int main() {
    sf::RenderWindow window(sf::VideoMode(param::game_width, param::game_height), "Space Invaders");

    // Initialize systems & pools
    GameSystem::init();
    Bullet::init();

    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        // Update systems
        GameSystem::update(dt);
        Bullet::update(dt);

        // Render systems
        window.clear(sf::Color::Black);
        GameSystem::render(window);
        Bullet::render(window);
        window.display();
    }

    GameSystem::clean();
    return 0;
}