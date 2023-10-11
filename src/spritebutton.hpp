#include "sprite.hpp"
#include <raylib-cpp.hpp>

class SpriteButton : public Sprite
{
  private:
    float scaleDefault = 1.0f;
    float scaleTarget = 1.2f;   // Target scale when the button is pressed
    float scaleDuration = 0.2f; // Duration of the scale animation
    float elapsedTime;

  public:
    using Sprite::Sprite;
    void ResetElapsedTime();
    void InitScale();
    enum class ButtonEvent
    {
        IDLE,                  // Only used when initializing the ButtonEvent to avoid triggering a press or release.
        IN_ANIM_PRESSED,       // Is currently in the press animation.
        IN_ANIM_RELEASED,      // Is currently in the release animation.
        FINISHED_ANIM_PRESSED, // Has finished the press animation (idle).
        FINISHED_ANIM_RELEASED // Has finished the release animation (idle).
    };
    ButtonEvent buttonEvent;
    void RefreshButtonScale(float delta, raylib::Camera2D &camera);
};
