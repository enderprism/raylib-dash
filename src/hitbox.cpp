#include "hitbox.hpp"
#include <iostream>

Hitbox::HitboxType Hitbox::GetType()
{
    return type;
}

void Hitbox::SetType(Hitbox::HitboxType new_type)
{
    type = new_type;
}

raylib::Rectangle Hitbox::GetBounds()
{
    return bounds;
}

void Hitbox::SetBounds(raylib::Rectangle new_bounds)
{
    bounds = new_bounds;
    isSpriteSize = false;
}
