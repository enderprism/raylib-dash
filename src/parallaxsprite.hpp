#include "playercamera.hpp"
#include "sprite.hpp"
#include <raylib-cpp.hpp>

class ParallaxSprite : public Sprite
{
  private:
    float parallaxFactorX = 0.0f;
    float parallaxFactorY = 0.0f;
    const float GROUND_POSITION_Y = 0.0f;
    raylib::Vector2 deltaCamera = Vector2Zero();

  public:
    using Sprite::Sprite;
    void SetParallaxFactor(float factorX, float factorY)
    {
        parallaxFactorX = factorX;
        parallaxFactorY = factorY;
    };
    void SetParallaxFactorX(float factorX) { parallaxFactorX = factorX; };
    void SetParallaxFactorY(float factorY) { parallaxFactorY = factorY; };
    float GetParallaxFactorX() { return parallaxFactorX; };
    float GetParallaxFactorY() { return parallaxFactorY; };
    void UpdateParallax(PlayerCamera &playerCamera);
};
