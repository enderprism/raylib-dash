#include "hitbox.hpp"
#include <iostream>

Hitbox::HitboxType Hitbox::GetType()
{
    return type;
}

raylib::Rectangle Hitbox::GetBounds()
{
    return bounds;
}
