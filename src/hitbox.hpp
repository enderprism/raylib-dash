#pragma once
#include <raylib-cpp.hpp>

class Hitbox
{
  public:
    enum HitboxType
    {
        SOLID,
        HAZARD,
        ORB,
        PAD,
        GAMEMODE_PORTAL,
        SPEED_PORTAL,
        SIZE_PORTAL,
        GRAVITY_PORTAL,
        TELEPORTAL,
        TRIGGER,
        NONSOLID,
        GROUND,
    };
    enum Orb
    {
        ORB_PINK = 1,
        ORB_YELLOW = 2,
        ORB_RED = 4,
        ORB_BLUE = 8,
        ORB_GREEN = 16,
        ORB_BLACK = 32,
        ORB_SPIDER = 64,
        ORB_TOGGLE = 128,
        ORB_REBOUND = 256,
        ORB_TELEPORT = 512,
        ORB_DASH_GREEN = 1024,
        ORB_DASH_MAGENTA = 2048,
    };
    enum Pad
    {
        PAD_PINK = 1,
        PAD_YELLOW = 2,
        PAD_RED = 4,
        PAD_BLUE = 8,
        PAD_GREEN = 16,
        PAD_BLACK = 32,
        PAD_SPIDER = 64,
        PAD_REBOUND = 128,
    };
    enum GamemodePortal
    {
        GMPORTAL_CUBE = 1,
        GMPORTAL_SHIP = 2,
        GMPORTAL_UFO = 4,
        GMPORTAL_BALL = 8,
        GMPORTAL_WAVE = 16,
        GMPORTAL_ROBOT = 32,
        GMPORTAL_SPIDER = 64,
        GMPORTAL_SWING = 128,
    };
    enum SpeedPortal
    {
        SPPORTAL_1X = 1,
        SPPORTAL_2X = 2,
        SPPORTAL_3X = 4,
        SPPORTAL_4X = 8,
        SPPORTAL_5X = 16,
        SPPORTAL_05X = 32,
    };
    enum SizePortal
    {
        SZPORTAL_NORMAL,
        SZPORTAL_SMALL,
        SZPORTAL_BIG, // hehe new feature
    };
    enum GravityPortal
    {
        GVPORTAL_BLUE,
        GVPORTAL_YELLOW,
        GVPORTAL_GREEN,
    };

    bool isSpriteSize = true;
    raylib::Rectangle bounds;
    HitboxType type;
    HitboxType GetType();
    void SetType(HitboxType new_type);
    raylib::Rectangle GetBounds();
    void SetBounds(raylib::Rectangle new_bounds);
};
