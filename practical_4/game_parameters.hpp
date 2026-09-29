#pragma once

struct Parameters {
    static constexpr int game_width = 800;
    static constexpr int game_height = 600;
    static constexpr int sprite_size = 32;

    static constexpr int rows = 5;
    static constexpr int columns = 11;

    static constexpr float bullet_speed = 300.0f;
    static constexpr float player_speed = 220.0f;
    static constexpr float invader_start_speed = 50.0f;
    static constexpr float invader_acc = 8.0f;
};