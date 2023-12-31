#include "sprite.hpp"
#include "hitbox.hpp"
#include "options.hpp"
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <string>

void Sprite::UpdateHitbox()
{
    if (hitbox.isSpriteSize)
    {
        hitbox.bounds = raylib::Rectangle{position.x, position.y, texture.GetWidth() * abs(scaleV.x), texture.GetHeight() * abs(scaleV.y)};
    }
    else
    {
        hitbox.bounds.x += position.x - hitbox.GetBounds().GetX();
        hitbox.bounds.y += position.y - hitbox.GetBounds().GetY();
        hitbox.bounds.width += texture.GetWidth() * abs(scaleV.x) - hitbox.GetBounds().GetWidth();
        hitbox.bounds.height += texture.GetHeight() * abs(scaleV.y) - hitbox.GetBounds().GetHeight();
    }
}

Sprite::Sprite(std::string self_texture_path, raylib::Vector2 self_position, float self_scale, float self_rotation, raylib::Color self_tint)
    : texture_path(self_texture_path), position(self_position), scale(self_scale), rotation(self_rotation), tint(self_tint)
{
    texture = LoadTexture(self_texture_path.c_str());
    // Set hitbox to sprite size
    scaleV = raylib::Vector2{scale, scale};
    hitbox.bounds = raylib::Rectangle{position.x, position.y, texture.GetWidth() * abs(scaleV.x), texture.GetHeight() * abs(scaleV.y)};

    texture.SetFilter(TEXTURE_FILTER_TRILINEAR);
    // texture.GenMipmaps();
    rectColor = raylib::Color((GetRandomValue(0, 255)), (GetRandomValue(0, 255)), (GetRandomValue(0, 255)), 50);
};

void Sprite::Draw()
{
    // texture = LoadTexture(texture_path.c_str());
    scaleV = raylib::Vector2{scale, scale};
    UpdateHitbox();
    debugRect.SetPosition(position);
    debugRect.SetWidth(texture.GetWidth() * abs(scale));
    debugRect.SetHeight(texture.GetHeight() * abs(scale));
    if (scale < 0)
    {
        debugRect.x -= texture.width * abs(scale);
        debugRect.y -= texture.height * abs(scale);
    }
    texture.Draw(
        // sourceRec
        raylib::Rectangle{
            0.0f, 0.0f,
            (float)texture.GetWidth() * flip.x,
            (float)texture.GetHeight() * flip.y},
        // destRec
        raylib::Rectangle{
            position.x + (float)texture.GetWidth() / 2 * scaleV.x,
            position.y + (float)texture.GetHeight() / 2 * scaleV.y,
            scaleV.x * (float)texture.GetWidth(),
            scaleV.y * (float)texture.GetHeight()},
        // origin x&y
        raylib::Vector2{
            (float)texture.GetWidth() / 2 * scaleV.x,
            (float)texture.GetHeight() / 2 * scaleV.y},
        rotation, tint);
    if (SHOW_BOXES)
    {
        debugRect.Draw(rectColor);
    }
};

void Sprite::Draw(raylib::Vector2 drawPosition)
{
    if (drawPosition.y == 0 && position.y != 0.0)
    {
        drawPosition.y = position.y;
    }
    scaleV = raylib::Vector2{scale, scale};
    UpdateHitbox();
    debugRect.SetPosition(drawPosition);
    debugRect.SetWidth(texture.GetWidth() * abs(scale));
    debugRect.SetHeight(texture.GetHeight() * abs(scale));
    if (scale < 0)
    {
        debugRect.x -= texture.width * abs(scale);
        debugRect.y -= texture.height * abs(scale);
    }
    texture.Draw(
        // sourceRec
        raylib::Rectangle{
            0.0f, 0.0f,
            (float)texture.GetWidth() * flip.x,
            (float)texture.GetHeight() * flip.y},
        // destRec
        raylib::Rectangle{
            drawPosition.x + (float)texture.GetWidth() / 2 * scaleV.x,
            drawPosition.y + (float)texture.GetHeight() / 2 * scaleV.y,
            scaleV.x * (float)texture.GetWidth(),
            scaleV.y * (float)texture.GetHeight()},
        // origin x&y
        raylib::Vector2{
            (float)texture.GetWidth() / 2 * scaleV.x,
            (float)texture.GetHeight() / 2 * scaleV.y},
        rotation, tint);
    if (SHOW_BOXES)
    {
        debugRect.Draw(rectColor);
    }
};

void Sprite::Draw(raylib::Vector2 drawPosition, float drawRotation)
{
    if (drawPosition.y == 0 && position.y != 0.0)
    {
        drawPosition.y = position.y;
    }
    scaleV = raylib::Vector2{scale, scale};
    UpdateHitbox();
    debugRect.SetPosition(drawPosition);
    debugRect.SetWidth(texture.GetWidth() * abs(scale));
    debugRect.SetHeight(texture.GetHeight() * abs(scale));
    if (scale < 0)
    {
        debugRect.x -= texture.width * abs(scale);
        debugRect.y -= texture.height * abs(scale);
    }
    texture.Draw(
        // sourceRec
        raylib::Rectangle{
            0.0f, 0.0f,
            (float)texture.GetWidth() * flip.x,
            (float)texture.GetHeight() * flip.y},
        // destRec
        raylib::Rectangle{
            drawPosition.x + ((float)texture.GetWidth() / 2) * scaleV.x,
            drawPosition.y + ((float)texture.GetHeight() / 2) * scaleV.y,
            scaleV.x * (float)texture.GetWidth(),
            scaleV.y * (float)texture.GetHeight()},
        // origin x&y
        raylib::Vector2{
            (float)texture.GetWidth() / 2 * scaleV.x,
            (float)texture.GetHeight() / 2 * scaleV.y},
        drawRotation, tint);
    if (SHOW_BOXES)
    {
        debugRect.Draw(rectColor);
    }
};

bool Sprite::IsHovered(raylib::Camera2D &camera)
{
    raylib::Rectangle translatedRect = raylib::Rectangle{
        GetWorldToScreen2D(position, camera).x,
        GetWorldToScreen2D(position, camera).y,
        (float)(texture.width) * camera.GetZoom(),
        (float)(texture.height) * camera.GetZoom()};
    if (translatedRect.CheckCollision(GetMousePosition()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

raylib::Vector2 Sprite::GetCenter()
{
    return raylib::Vector2{
        position.x + (float)(texture.width) * scale * 0.5f,
        position.y + (float)(texture.height) * scale * 0.5f};
}

// Get percentage of the width and height of the game window from the sprite's position, width, height and scale.
raylib::Vector2 Sprite::GetWinPercentage()
{
    return raylib::Vector2{
        GetCenter().x / GetMonitorWidth(MONITOR),
        GetCenter().y / GetMonitorHeight(MONITOR)};
}

// Get percentage of the width and height of the specified rectangle from the sprite's position, width, height and scale.
raylib::Vector2 Sprite::GetRecPercentage(raylib::Rectangle rec)
{
    return raylib::Vector2{
        GetCenter().x / rec.width,
        GetCenter().y / rec.height};
}

// Get percentage of the width and height of the specified rectangle from the sprite's position.
raylib::Vector2 Sprite::GetRecPercentageUnaligned(raylib::Rectangle rec)
{
    return raylib::Vector2{
        position.x / rec.width,
        position.y / rec.height};
}

// Move Sprite to percentage of the width and height of the game window.
raylib::Vector2 Sprite::MoveToWinPercentage(raylib::Vector2 percentage)
{
    return raylib::Vector2{
        (GetMonitorWidth(MONITOR) * percentage.x) - ((float)texture.width / 2) * scale,
        (GetMonitorHeight(MONITOR) * percentage.y) - ((float)texture.height / 2) * scale};
}

// Move Sprite to percentage of the width and height of the specified Rectangle.
raylib::Vector2 Sprite::MoveToRecPercentage(raylib::Vector2 percentage, raylib::Rectangle rec)
{
    return raylib::Vector2{
        rec.GetX() + (rec.width * percentage.x) - ((float)texture.width / 2) * scale,
        rec.GetY() + (rec.height * percentage.y) - ((float)texture.height / 2) * scale};
}

// Move Sprite to percentage of the width and height of the specified Rectangle without centering the texture.
raylib::Vector2 Sprite::MoveToRecPercentageUnaligned(raylib::Vector2 percentage, raylib::Rectangle rec)
{
    return raylib::Vector2{
        rec.GetX() + (rec.width * percentage.x),
        rec.GetY() + (rec.height * percentage.y)};
}
