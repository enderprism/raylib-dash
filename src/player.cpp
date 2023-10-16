#include "player.hpp"
#include "hitbox.hpp"
#include <iostream>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <raymath.h>

raylib::Vector2 Player::GetDirection(bool onGround)
{
    raylib::Vector2 result;
    bool isJumping = IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP) || IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if (isJumping && (onGround || orb_collisions != 0 || pad_collisions != 0))
    {
        result.y = 1;
    }
    else
    {
        result.y = -1;
    }
    if (isPlatformer)
    {
        result.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
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

float Player::CalculateIconRotation(float delta)
{
    if (!isPlatformer)
    {
        if (gamemode == Gamemode::CUBE || gamemode == Gamemode::BALL)
        {
            if (collidingSides & CollidingSides::GROUND)
            {
                return Lerp(rotation, roundf(rotation / 90) * 90, 0.5f);
            }
            else
            {
                return rotation + delta * 400.0f * direction.x;
            }
        }
    }
    return rotation;
}

void Player::KillPlayer(float delta)
{
    std::cout << "The player is DEAD!! No but seriously who killed player?" << std::endl;
}

raylib::Vector2 Player::GetPlayerOutOfGround(raylib::Rectangle ground)
{
    collidingSides |= CollidingSides::GROUND;
    return raylib::Vector2{position.x, (ground.GetY() - bigHitbox.GetHeight())};
}

raylib::Vector2 Player::CalculateVelocity(float delta)
{
    float verticalVelocity;
    float horizontalVelocity;
    if (!verticalGravity)
    {
        horizontalVelocity = velocity.x;
        verticalVelocity = velocity.y;
    }
    else
    {
        horizontalVelocity = velocity.y;
        verticalVelocity = velocity.x;
    }
    // Gravity calculation

    if (isPlatformer)
    {
        horizontalVelocity = GetHorizontalVelocityWithFriction(delta, velocity.x);
    }
    else
    {
        horizontalVelocity = horizontal_direction * speed.x * delta;
    }

    if (!(collidingSides & CollidingSides::GROUND))
    {
        // std::cout << GetTime() << " ONGROUND at " << position.y << std::endl;
        verticalVelocity += gravity * delta * 0.25;
    }
    else
    {
        verticalVelocity = 0.0f;
    }
    if (gamemode != Gamemode::CUBE)
    {
        verticalVelocity = Clamp(verticalVelocity, -max_speed.GetY(), max_speed.GetY());
    }

    if (direction.y == 1)
    {
        collidingSides &= ~CollidingSides::GROUND;
        if (orb_collisions & Hitbox::Orb::ORB_YELLOW)
        {
            verticalVelocity -= speed.y * delta * 1.2f;
        }
        else
        {
            verticalVelocity -= speed.y * delta;
        }
    }

    // std::cout << verticalVelocity << std::endl;
    if (!verticalGravity)
    {
        return raylib::Vector2{horizontalVelocity, verticalVelocity};
    }
    else
    {
        return raylib::Vector2{verticalVelocity, horizontalVelocity};
    }
}

void Player::UpdatePlayer(float delta)
{
    textureRec = raylib::Rectangle{position.x, position.y, texture.GetWidth() * scaleV.x, texture.GetHeight() * scaleV.y};
    bigHitbox.SetPosition(
        MoveToRecPercentageUnaligned(Vector2Zero(), textureRec));
    bigHitbox.SetSize(
        MoveToRecPercentageUnaligned(Vector2One(), textureRec) - MoveToRecPercentageUnaligned(Vector2Zero(), textureRec));
    bool bigHitboxCollidingLeft = false;
    bool bigHitboxCollidingRight = false;
    bool bigHitboxCollidingTop = false;
    bool bigHitboxCollidingBottom = false;
    bool standingOnGround = false;

    // std::cout << envItems.size() << std::endl;
    // Get direction
    direction = GetDirection(collidingSides & CollidingSides::GROUND);

    velocity = CalculateVelocity(delta);
    rotation = CalculateIconRotation(delta);
    position += velocity;
    // std::cout << bigHitbox.y << " " << velocity.y << " " << (collidingSides & CollidingSides::GROUND) << std::endl;

    for (Hitbox &object : envItems)
    {
        // std::cout << (bigHitbox.y + bigHitbox.height > object.bounds.y) << std::endl;
        // Collisions from the player's hitbox
        bigHitboxCollidingLeft = bigHitbox.x < object.bounds.x + object.bounds.width;
        bigHitboxCollidingRight = bigHitbox.x + bigHitbox.width > object.bounds.x;
        bigHitboxCollidingTop = bigHitbox.y < object.bounds.y + object.bounds.height;
        bigHitboxCollidingBottom = bigHitbox.y + bigHitbox.height > object.bounds.y;
        standingOnGround = (bigHitbox.y + bigHitbox.height + 1 > object.bounds.y) && (object.GetType() == Hitbox::HitboxType::SOLID);

        // bool smallHitboxCollidingLeft = smallHitbox.x < object.bounds.x + object.bounds.width;
        // bool smallHitboxCollidingRight = smallHitbox.x + smallHitbox.width > object.bounds.x;
        // bool smallHitboxCollidingTop = smallHitbox.y < object.bounds.y + object.bounds.height;
        // bool smallHitboxCollidingBottom = smallHitbox.y + smallHitbox.height > object.bounds.y;

        if (bigHitboxCollidingBottom && object.GetType() == Hitbox::HitboxType::SOLID)
        {
            // Get the player out of the ground
            std::cout << "Getting the player out of the ground\n";
            position = GetPlayerOutOfGround(object.GetBounds());
        }
        //
        // if ((bigHitboxCollidingLeft || bigHitboxCollidingRight) && object.GetType() == Hitbox::HitboxType::SOLID)
        // {
        //     if (isPlatformer)
        //     {
        //         velocity.x = 0.0;
        //     }
        //     else
        //     {
        //         KillPlayer(delta);
        //     }
        // }
        // if (bigHitboxCollidingTop && object.GetType() == Hitbox::HitboxType::SOLID)
        // {
        //     if (isPlatformer)
        //     {
        //         velocity.y = 0.0;
        //     }
        //     else
        //     {
        //         KillPlayer(delta);
        //     }
        // }

        if (CheckCollisionRecs(bigHitbox, object.bounds) && object.GetType() == Hitbox::HitboxType::HAZARD)
        {
            KillPlayer(delta);
        }
        if (CheckCollisionRecs(smallHitbox, object.bounds) && object.GetType() == Hitbox::HitboxType::SOLID)
        {
            KillPlayer(delta);
        }

        if (collidingSides & CollidingSides::GROUND && !standingOnGround)
        {
            collidingSides &= ~CollidingSides::GROUND;
        }
        // etc.
    };
};
