#pragma once
#include "hitbox.hpp"
#include <Music.hpp>
#include <raylib-cpp.hpp>
#include <vector>

class Level
{
  public:
    std::vector<std::reference_wrapper<Hitbox>> objects;
    raylib::Music song;
    float songStartTime;
    enum class Difficulty
    {
        AUTO,
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
    Difficulty difficulty = Difficulty::EASY;
    int starCount = 1;
};
