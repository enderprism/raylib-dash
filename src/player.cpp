#include "player.hpp"
#include "hitbox.hpp"
#include <raylib-cpp.hpp>

raylib::Vector2 Player::GetDirection()
{
    raylib::Vector2 result;
    if (isPlatformer)
    {
        result.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
        result.y = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP); // Temporary top-down controls
        horizontal_direction = result.x;
        jump_direction = result.y;
    }
    else
    {
        result.x = horizontal_direction;
    }
    return result;
}

float Player::GetHorizontalVelocityWithFriction(float delta, float horizontal_velocity)
{
    // Friction calculation

    if (direction.x != 0.0)
    {
        horizontal_velocity = direction.x * speed.x * delta;
        flip.x = direction.x;
    }
    if (direction.x == 0)
    {
        // Threshold is set to 1 pixel because there are no sub-pixel movements
        if (horizontal_velocity > 1)
        {
            horizontal_velocity -= friction * delta;
        }
        else if (horizontal_velocity < -1)
        {
            horizontal_velocity += friction * delta;
        }
        else
        {
            horizontal_velocity = 0;
        }
    }
    return horizontal_velocity;
}

void Player::UpdatePlayer(float delta)
{
    // Get direction
    direction = GetDirection();

    // Friction calculation
    if (isPlatformer)
    {
        velocity.x = GetHorizontalVelocityWithFriction(delta, velocity.x);
    }

    for (int i; i < envItems.size(); i++)
    {
        // Collisions from the player's hitbox
        bool bigHitboxCollidingLeft = bigHitbox.x < envItems[i].bounds.x + envItems[i].bounds.width;
        bool bigHitboxCollidingRight = bigHitbox.x + bigHitbox.width > envItems[i].bounds.x;
        bool bigHitboxCollidingTop = bigHitbox.y < envItems[i].bounds.y + envItems[i].bounds.height;
        if ((bigHitboxCollidingLeft || bigHitboxCollidingRight) && envItems[i].GetType() == Hitbox::HitboxType::SOLID)
        {
            if (isPlatformer)
            {
                velocity.x = 0.0;
            }
            else
            {
                KillPlayer(delta);
            }
        }
        if (bigHitboxCollidingTop && envItems[i].GetType() == Hitbox::HitboxType::SOLID)
        {
            if (isPlatformer)
            {
                velocity.y = 0.0;
            }
            else
            {
                KillPlayer(delta);
            }
        }
        if (CheckCollisionRecs(bigHitbox, envItems[i].bounds) && envItems[i].GetType() == Hitbox::HitboxType::HAZARD)
        {
            KillPlayer(delta);
        }
        // etc.
    };
    position += velocity;
};
