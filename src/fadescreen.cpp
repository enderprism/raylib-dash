#include "fadescreen.hpp"
#include "options.hpp"
#include "tween.hpp"
#include <raylib-cpp.hpp>

FadeScreen::FadeScreen(raylib::Color self_color)
    : color(self_color)
{
    rec.SetPosition(0.0f, 0.0f);
    rec.SetSize(GetMonitorWidth(MONITOR), GetMonitorHeight(MONITOR));
};

void FadeScreen::Draw()
{
    color.a = alpha;
    rec.Draw(color);
};

void FadeScreen::FadeIn(float duration)
{
    fadeEvent = FadeEvent::FADING_IN;
    fadeDuration = duration;
    elapsedTime = 0.0f;
};

void FadeScreen::FadeOut(float duration)
{
    fadeEvent = FadeEvent::FADING_OUT;
    fadeDuration = duration;
    elapsedTime = 0.0f;
};

void FadeScreen::RefreshFade(float delta)
{
    if (fadeEvent == FadeEvent::FADING_OUT)
    {
        if (alpha > alphaDefault)
        {
            alpha = Lerp(alphaTarget, alphaDefault, tween::sinein(elapsedTime / fadeDuration));
            elapsedTime += delta;
        }
        else
        {
            fadeEvent = FadeEvent::FADED_OUT;
        }
    }
    else if (fadeEvent == FadeEvent::FADING_IN)
    {
        if (alpha < alphaTarget)
        {
            alpha = Lerp(alphaDefault, alphaTarget, tween::sinein(elapsedTime / fadeDuration));
            elapsedTime += delta;
        }
        else
        {
            fadeEvent = FadeEvent::FADED_IN;
        }
    }
}
