#include <raylib-cpp.hpp>

class FadeScreen
{
  private:
    raylib::Rectangle rec;
    raylib::Color color;
    int alpha = 0;
    int alphaDefault = 0;
    int alphaTarget = 255;
    float fadeDuration = 0.5f;
    float elapsedTime;

  public:
    int GetAlpha() { return alpha; };
    enum class FadeEvent
    {
        IDLE,       // Only used when initializing the FadeEvent to avoid triggering a fade-in or fade-out.
        FADING_IN,  // Is currently fading in.
        FADING_OUT, // Is currently fading out.
        FADED_IN,   // Has finished fading in (idle).
        FADED_OUT   // Has finished fading out (idle).
    };
    FadeEvent fadeEvent;
    FadeScreen(raylib::Color self_color);
    void Draw();
    void FadeIn(float duration);
    void FadeOut(float duration);
    void RefreshFade(float delta);
};
