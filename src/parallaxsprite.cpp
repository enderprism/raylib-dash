#include "parallaxsprite.hpp"
#include "options.hpp"
#include <raylib-cpp.hpp>

ParallaxSprite::ParallaxSprite(Sprite self_sprite) : sprite(self_sprite){};

void ParallaxSprite::UpdateParallax(PlayerCamera &playerCamera)
{
    // Actual parallax code
    if (parallaxFactorX != 0.0f)
    {
        deltaCamera.x = playerCamera.deltaTarget.x - playerCamera.deltaOffset.x;
        sprite.position.x += deltaCamera.x * parallaxFactorX;
    }
    // std::cout << GetScreenToWorld2D(Vector2{0.0f, (float)GetMonitorHeight(MONITOR)}, playerCamera.GetCamera()).y << std::endl;
    if (parallaxFactorY != 0.0f)
    {
        if (GetScreenToWorld2D(Vector2{0.0f, (float)GetMonitorHeight(MONITOR)}, playerCamera.GetCamera()).y < GROUND_POSITION_Y)
        {
            deltaCamera.y = playerCamera.deltaTarget.y - playerCamera.deltaOffset.y;
            sprite.position.y += deltaCamera.y * parallaxFactorY;
        }
        else
        {
            sprite.position.y = GROUND_POSITION_Y - sprite.texture.GetHeight() * sprite.scaleV.x;
        }
    }

    // Scroll the background if it overflows, to give the impression of an infinite background.
    if ((sprite.position.x + sprite.texture.width * sprite.scaleV.x) < playerCamera.camera.GetScreenToWorld(Vector2Zero()).x)
    {
        sprite.position.x += (sprite.texture.width * sprite.scaleV.x);
    }
    if (sprite.position.x > playerCamera.camera.GetScreenToWorld(raylib::Vector2{(float)GetMonitorWidth(MONITOR), 0.0f}).x)
    {
        sprite.position.x -= (sprite.texture.width * sprite.scaleV.x);
    }
};
