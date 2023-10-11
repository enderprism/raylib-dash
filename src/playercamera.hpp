#pragma once
#include "player.hpp"
#include <raylib-cpp.hpp>

class PlayerCamera
{
  private:
    const float GROUND_HEIGHT = 225.0f;
    const float MIN_HEIGHT = -5000.0f;
    const float MAX_DIST = 200.0f; // Max vertical distance from the player.
    float dist;                    // Camera's vertical distance from the player.
    bool freefly = true;

  public:
    raylib::Camera2D camera = raylib::Camera2D{Vector2Zero(), Vector2Zero()};
    raylib::Camera2D GetCamera() { return camera; }
    float GetGroundHeight() { return GROUND_HEIGHT; }
    // TODO Will need to add a bool to determine if the target is the player or a static position.
    raylib::Vector2 target;
    raylib::Vector2 prevTarget;
    raylib::Vector2 deltaTarget;
    raylib::Vector2 offset;
    raylib::Vector2 prevOffset;
    raylib::Vector2 deltaOffset;
    bool is_static_x = false;
    bool is_static_y = false;
    Player &player;
    float zoom = 1.5f;
    float rotation = 0.0f; // in degrees
    PlayerCamera(Player &self_player);
    raylib::Vector2 GetCenter();
    void UpdateCamera();
};
