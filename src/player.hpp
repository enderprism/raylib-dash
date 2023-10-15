#pragma once
#include "hitbox.hpp"
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
    int collidingSides;            // Bit flag
    raylib::Rectangle bigHitbox;   // Big hitbox used for the ground and H blocks (in non-platformer) or all the Solid hiboxes (in platformer)
    raylib::Rectangle smallHitbox; // Small hitbox used to check collisions. If it collides with solids or hazards, the player dies.
    std::vector<Hitbox> envItems;
    using Sprite::Sprite;
    float GetHorizontalVelocityWithFriction(float delta, float horizontal_velocity);
    raylib::Vector2 GetDirection();
    void UpdatePlayer(float delta);
    void KillPlayer(float delta);
};
