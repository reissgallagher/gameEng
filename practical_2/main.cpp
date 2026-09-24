#include <SFML/Graphics.hpp>

const sf::Keyboard::Key controls[4] = {
    sf::Keyboard::A,   // Player1 UP
    sf::Keyboard::Z,   // Player1 Down
    sf::Keyboard::Up,  // Player2 UP
    sf::Keyboard::Down // Player2 Down
};

// Parameters
const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f; // 60 fps

// Pong 3 Parameters
sf::Vector2f ball_velocity;
bool is_player_serving = true;
const float initial_velocity_x = 100.f;
const float initial_velocity_y = 60.f;
const float velocity_multiplier = 1.1f;

int scorePlayer1 = 0;
int scorePlayer2 = 0;

// Objects of the game
sf::CircleShape ball;
sf::RectangleShape paddles[2];
sf::Font font;
sf::Text text;

void reset() {
    // Reset paddle positions
    paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f);
    paddles[1].setPosition(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f);

    // Reset ball position
    ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);

    // Ball velocity based on who is serving
    ball_velocity = { (is_player_serving ? initial_velocity_x : -initial_velocity_x), initial_velocity_y };

    // Update Score Text
    text.setString(std::to_string(scorePlayer1) + " : " + std::to_string(scorePlayer2));
    text.setPosition((gameWidth * 0.5f) - (text.getLocalBounds().width * 0.5f), 10.f);
}

void init() {
    // Set size and origin of paddles
    for (sf::RectangleShape &p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }

    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius);

    // Font and Text setup
    font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    reset();
}

void update(float dt) {
    // Player 1 movement
    float direction1 = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[0])) {
        direction1--;
    }
    if (sf::Keyboard::isKeyPressed(controls[1])) {
        direction1++;
    }
    paddles[0].move(sf::Vector2f(0.f, direction1 * paddleSpeed * dt));

    // Player 2 movement
    float direction2 = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[2])) {
        direction2--;
    }
    if (sf::Keyboard::isKeyPressed(controls[3])) {
        direction2++;
    }
    paddles[1].move(sf::Vector2f(0.f, direction2 * paddleSpeed * dt));

    // Paddle screen boundary validation
    for (sf::RectangleShape &p : paddles) {
        float halfPaddleH = paddleSize.y / 2.f;
        if (p.getPosition().y - halfPaddleH < 0.f) {
            p.setPosition(p.getPosition().x, halfPaddleH);
        } else if (p.getPosition().y + halfPaddleH > gameHeight) {
            p.setPosition(p.getPosition().x, gameHeight - halfPaddleH);
        }
    }

    // Ball movement
    ball.move(ball_velocity * dt);

    // Ball collision checks
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;

    if (by > gameHeight) { // Bottom wall
        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;
        ball.move(sf::Vector2f(0.f, -10.f));
    } else if (by < 0) { // Top wall
        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;
        ball.move(sf::Vector2f(0.f, 10.f));
    } else if (bx > gameWidth) { // Right wall (Player 1 scores)
        scorePlayer1++;
        is_player_serving = false;
        reset();
    } else if (bx < 0) { // Left wall (Player 2 scores)
        scorePlayer2++;
        is_player_serving = true;
        reset();
    } else if (
        // Bounce off Left Paddle
        bx < paddleSize.x + paddleOffsetWall &&
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5f) &&
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5f)
    ) {
        ball_velocity.x *= -velocity_multiplier;
        ball_velocity.y *= velocity_multiplier;
        ball.move(sf::Vector2f(10.f, 0.f));
    } else if (
        // Bounce off Right Paddle
        bx > gameWidth - (paddleSize.x + paddleOffsetWall) &&
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5f) &&
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5f)
    ) {
        ball_velocity.x *= -velocity_multiplier;
        ball_velocity.y *= velocity_multiplier;
        ball.move(sf::Vector2f(-10.f, 0.f));
    }
}

void render(sf::RenderWindow &window) {
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
    window.draw(text);
}

void clean() {
    // Free up memory if necessary
}

int main() {
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "PONG");
    init();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        static sf::Clock clock;
        const float dt = clock.restart().asSeconds();

        window.clear();
        update(dt);
        render(window);

        sf::sleep(sf::seconds(time_step));
        window.display();
    }

    clean();
    return 0;
}