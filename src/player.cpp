#include "player.hpp"
#include <raylib-cpp.hpp>

void Player::GetCollision(raylib::Rectangle usedHitbox)
{
    collidingSides = 0;
    for (int i; i < envItems.size(); i++)
    {
        // Collisions from the player's hitbox
        // Left collision
        if (usedHitbox.x < envItems[i].x + envItems[i].width)
        {
            collidingSides |= (int)CollidingSides::LEFT;
        }
        // etc.
    };
};

void Player::UpdatePlayer(float delta)
{
    // Get direction
    if (isPlatformer)
    {
        direction.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
        horizontal_direction = direction.x;
    }
    else
    {
        direction.x = horizontal_direction;
    }

    // Friction calculation

    if (direction.x != 0.0)
    {
        velocity.x = direction.x * speed.x * delta;
        flip.x = direction.x;
    }
    if (direction.x == 0)
    {
        // Threshold is set to 1 pixel because there are no sub-pixel movements
        if (velocity.x > 1)
        {
            velocity.x -= friction * delta;
        }
        else if (velocity.x < -1)
        {
            velocity.x += friction * delta;
        }
        else
        {
            velocity.x = 0;
        }
    }
    position.x += velocity.x;

    // DEBUG: Temporary top-down controls
    // Get direction
    if (isPlatformer)
    {
        direction.y = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);
        jump_direction = direction.y;
    }

    // Friction calculation

    if (direction.y != 0.0)
    {
        velocity.y = direction.y * speed.y * delta;
    }
    if (direction.y == 0)
    {
        // Threshold is set to 1 pixel because there are no sub-pixel movements
        if (velocity.y > 1)
        {
            velocity.y -= friction * delta;
        }
        else if (velocity.y < -1)
        {
            velocity.y += friction * delta;
        }
        else
        {
            velocity.y = 0;
        }
    }
    position.y += velocity.y;
};
