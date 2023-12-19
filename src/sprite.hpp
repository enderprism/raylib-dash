#pragma once
#include "hitbox.hpp"
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <string>

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
    Sprite(std::string self_texture_path, raylib::Vector2 self_position = raylib::Vector2{0.0, 0.0}, float self_scale = 1.0f, float self_rotation = 0.0f, raylib::Color self_tint = WHITE);
    Sprite(const Sprite &prev_sprite) : Sprite{prev_sprite.texture_path, prev_sprite.position, prev_sprite.scale, prev_sprite.rotation, prev_sprite.tint} {
                                            // texture = Texture2D(prev_sprite.texture);
                                        }; // copy constructor
    void Draw();
    void Draw(raylib::Vector2 drawPosition);
    void Draw(raylib::Vector2 drawPosition, float drawRotation);
    bool IsHovered(raylib::Camera2D &camera);
    raylib::Vector2 MoveToWinPercentage(raylib::Vector2 percentage);
    raylib::Vector2 MoveToRecPercentage(raylib::Vector2 percentage, raylib::Rectangle rec);
    raylib::Vector2 MoveToRecPercentageUnaligned(raylib::Vector2 percentage, raylib::Rectangle rec);
    raylib::Vector2 GetWinPercentage();
    raylib::Vector2 GetRecPercentage(raylib::Rectangle rec);
    raylib::Vector2 GetRecPercentageUnaligned(raylib::Rectangle rec);
    raylib::Vector2 GetCenter();
    void UpdateHitbox();
    std::string texture_path; // load texture in constructor
};
