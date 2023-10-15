#include "sprite.hpp"
#include <Music.hpp>
#include <raylib-cpp.hpp>
#include <vector>

class Level
{
  public:
    std::vector<Sprite> objects;
    raylib::Music song;
    float songStartTime;
    enum class Difficulty
    {
        EASY,
        NORMAL,
        HARD,
        HARDER,
        INSANE,
        EASY_DEMON,
        MEDIUM_DEMON,
        HARD_DEMON,
        INSANE_DEMON,
        EXTREME_DEMON,
    };
    Difficulty difficulty;
    int starCount;
};
