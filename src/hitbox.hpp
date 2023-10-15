#pragma once
#include <raylib-cpp.hpp>

class Hitbox
{
  public:
    enum class HitboxType
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
    };
    enum class Orb
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
    enum class Pad
    {
        PAD_PINK = 1,
        PAD_YELLOW = 2,
        PAD_RED = 4,
        PAD_BLUE = 8,
        PAD_GREEN = 16,
        PAD_BLACK = 32,
        PAD_SPIDER = 64,
        PAD_TOGGLE = 128,
        PAD_REBOUND = 256,
    };
    enum class GamemodePortal
    {
        GMPORTAL_CUBE = 1,
        GMPORTAL_SHIP = 2,
        GMPORTAL_UFO = 4,
        GMPORTAL_WAVE = 8,
        GMPORTAL_ROBOT = 16,
        GMPORTAL_SPIDER = 32,
        GMPORTAL_SWINGCOPTER = 64,
    };
    enum class SpeedPortal
    {
        SPPORTAL_1X = 1,
        SPPORTAL_2X = 2,
        SPPORTAL_3X = 4,
        SPPORTAL_4X = 8,
        SPPORTAL_5X = 16,
        SPPORTAL_05X = 32,
    };
    enum class SizePortal
    {
        SZPORTAL_NORMAL,
        SZPORTAL_SMALL,
        SZPORTAL_BIG, // hehe new feature
    };
    enum class GravityPortal
    {
        GVPORTAL_BLUE,
        GVPORTAL_YELLOW,
        GVPORTAL_GREEN,
    };

    raylib::Rectangle bounds;
    HitboxType type;
    HitboxType GetType();
    raylib::Rectangle GetBounds();
};
