#include "spritebutton.hpp"
#include "tween.hpp"
#include <raylib-cpp.hpp>

SpriteButton::SpriteButton(Sprite self_sprite) : sprite(self_sprite) {}

void SpriteButton::ResetElapsedTime() { elapsedTime = 0.0f; };

void SpriteButton::InitScale()
{
    scaleDefault *= sprite.scale;
    scaleTarget *= sprite.scale;
}

void SpriteButton::RefreshButtonScale(float delta, raylib::Camera2D &camera)
{
    if (sprite.IsHovered(camera) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        buttonEvent = ButtonEvent::IN_ANIM_PRESSED;
        elapsedTime = 0.0f;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && (buttonEvent == ButtonEvent::IN_ANIM_PRESSED || buttonEvent == ButtonEvent::FINISHED_ANIM_PRESSED))
    {
        buttonEvent = ButtonEvent::IN_ANIM_RELEASED;
        elapsedTime = 0.0f;
    }
    if (buttonEvent == ButtonEvent::IN_ANIM_RELEASED)
    {
        if (sprite.scale > scaleDefault)
        {
            sprite.scale = Lerp(scaleTarget, scaleDefault, tween::bounceout(elapsedTime / scaleDuration));
            elapsedTime += delta;
        }
        else
        {
            buttonEvent = ButtonEvent::FINISHED_ANIM_RELEASED;
        }
    }
    else if (buttonEvent == ButtonEvent::IN_ANIM_PRESSED)
    {
        if (sprite.scale < scaleTarget)
        {
            sprite.scale = Lerp(scaleDefault, scaleTarget, tween::bounceout(elapsedTime / scaleDuration));
            elapsedTime += delta;
        }
        else
        {
            buttonEvent = ButtonEvent::FINISHED_ANIM_PRESSED;
        }
    }
}
