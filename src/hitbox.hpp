#include <raylib-cpp.hpp>

class Hitbox
{
  public:
    enum class HitboxType
    {
        SOLID,
        SPIKE,
        ORB,
        GAMEMODE_PORTAL,
        SPEED_PORTAL,
        TRIGGER,
    };
    raylib::Rectangle bounds;
    HitboxType type;
    void PrintHello();
    HitboxType GetType();
    raylib::Rectangle GetBounds();
};
