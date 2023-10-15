#pragma once
#include "hitbox.hpp"
#include <raylib-cpp.hpp>

class Sprite
{
  public:
    raylib::Texture2D texture;
    raylib::Rectangle debugRect;
    raylib::Color rectColor;
    raylib::Vector2 position;
    float scale;                 // Scale as a float (equal X and Y scale).
    raylib::Vector2 scaleV;      // Scale as a raylib::Vector2.
    raylib::Rectangle sourceRec; // Example usage: when you have a spritesheet that you need to split.
    float rotation;
    raylib::Color tint;
    raylib::Vector2 flip = raylib::Vector2{1.0f, 1.0f};
    Hitbox hitbox;
    Sprite(Texture2D self_texture, raylib::Vector2 self_position = raylib::Vector2{0.0, 0.0}, float self_scale = 1.0f, float self_rotation = 0.0f, raylib::Color self_tint = WHITE);
    void Draw();
    void Draw(raylib::Vector2 drawPosition);
    void Draw(raylib::Vector2 drawPosition, float drawRotation);
    bool IsHovered(raylib::Camera2D &camera);
    raylib::Vector2 MoveToWinPercentage(raylib::Vector2 percentage);
    raylib::Vector2 MoveToWinPercentage(raylib::Vector2 percentage, raylib::Rectangle rec);
    raylib::Vector2 GetWinPercentage();
    raylib::Vector2 GetWinPercentage(raylib::Rectangle rec);
    raylib::Vector2 GetCenter();
};
