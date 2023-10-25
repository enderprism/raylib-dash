#pragma once
#include "hitbox.hpp"
#include "sprite.hpp"
#include <functional>
#include <raylib-cpp.hpp>

// All values are taken from Godot Dash and are mapped to the player's height in Raylib Dash: (value*64)/122

class Player : public Sprite
{
  public:
    float time_scale = 1.0f;
    raylib::Rectangle textureRec;
    raylib::Vector2 speed{625.0f, 1100.0f};
    raylib::Vector2 max_speed{10000.0f, 1500.0f};
    raylib::Vector2 velocity;
    bool isPlatformer = true;
    raylib::Vector2 direction;
    int horizontal_direction = 1; // -1 = left, 1 = right
    bool flipped_gravity;
    int orb_collisions = 0;
    int pad_collisions = 0;
    int jump_direction = 0;
    const float gravity = 400.0f;
    const float friction = 625.0f;
    bool verticalGravity;
    int collidingSides; // Bit flag
    enum CollidingSides
    {
        GROUND = 1,
        CEILING = 2,
        LEFT = 4,
        RIGHT = 8,
    };
    enum Gamemode
    {
        CUBE,
        ROBOT,
        UFO,
        BALL,
        SHIP,
        WAVE,
        SPIDER,
        SWING,
    };
    Gamemode gamemode = Gamemode::CUBE;
    raylib::Rectangle bigHitbox;   // Big hitbox used for the ground and H blocks (in non-platformer) or all the Solid hiboxes (in platformer)
    raylib::Rectangle smallHitbox; // Small hitbox used to check collisions. If it collides with solids or hazards, the player dies.
    // std::vector<std::reference_wrapper<Hitbox>> levelObjects;
    using Sprite::Sprite;
    float GetHorizontalVelocityWithFriction(float delta, float horizontal_velocity);
    raylib::Vector2 GetDirection(bool onGround);
    void UpdatePlayer(float delta, std::vector<std::reference_wrapper<Hitbox>> &levelObjects);
    void KillPlayer(float delta);
    raylib::Vector2 GetPlayerOutOfGround(raylib::Rectangle ground);
    raylib::Vector2 CalculateVelocity(float delta);
    float CalculateIconRotation(float delta);
};
