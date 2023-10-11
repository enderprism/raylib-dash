#include "parallaxsprite.hpp"
#include "options.hpp"
#include <raylib-cpp.hpp>

void ParallaxSprite::UpdateParallax(PlayerCamera &playerCamera)
{
    // Actual parallax code
    if (parallaxFactorX != 0.0f)
    {
        deltaCamera.x = playerCamera.deltaTarget.x - playerCamera.deltaOffset.x;
        position.x += deltaCamera.x * parallaxFactorX;
    }
    // std::cout << GetScreenToWorld2D(Vector2{0.0f, (float)GetMonitorHeight(MONITOR)}, playerCamera.GetCamera()).y << std::endl;
    if (parallaxFactorY != 0.0f)
    {
        if (GetScreenToWorld2D(Vector2{0.0f, (float)GetMonitorHeight(MONITOR)}, playerCamera.GetCamera()).y < GROUND_POSITION_Y)
        {
            deltaCamera.y = playerCamera.deltaTarget.y - playerCamera.deltaOffset.y;
            position.y += deltaCamera.y * parallaxFactorY;
        }
        else
        {
            position.y = GROUND_POSITION_Y - texture.GetHeight() * scaleV.x;
        }
    }

    // Scroll the background if it overflows, to give the impression of an infinite background.
    if ((position.x + texture.width * scaleV.x) < playerCamera.camera.GetScreenToWorld(Vector2Zero()).x)
    {
        position.x += (texture.width * scaleV.x);
    }
    if (position.x > playerCamera.camera.GetScreenToWorld(raylib::Vector2{(float)GetMonitorWidth(MONITOR), 0.0f}).x)
    {
        position.x -= (texture.width * scaleV.x);
    }
};
