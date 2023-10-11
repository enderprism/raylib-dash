#pragma once
#include "sprite.hpp"
#include <raylib-cpp.hpp>

class Player : public Sprite
{
  public:
    raylib::Vector2 speed{625.0f, 625.0f};
    raylib::Vector2 velocity;
    bool isPlatformer = true;
    raylib::Vector2 direction;
    int horizontal_direction = 1; // -1 = left, 1 = right
    int jump_direction = 0;
    const float gravity = 5000;
    const float friction = 625.0f;
    int collidingSides; // Bit flag
    std::vector<raylib::Rectangle> envItems;
    using Sprite::Sprite;
    void GetCollision(raylib::Rectangle usedHitbox);
    void UpdatePlayer(float delta);
    enum class CollidingSides
    {
        LEFT = 1,
        UP = 2,
        RIGHT = 4,
        DOWN = 8,
    };
};
