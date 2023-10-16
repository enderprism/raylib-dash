#include "fadescreen.hpp"
#include "hitbox.hpp"
#include "options.hpp"
#include "parallaxsprite.hpp"
#include "player.hpp"
#include "playercamera.hpp"
#include "sprite.hpp"
#include "spritebutton.hpp"
#include <Rectangle.hpp>
#include <Vector2.hpp>
#include <algorithm>
#include <iostream>
#include <raylib-cpp.hpp>

// TODO: Add triggers.

template <typename T>
void UpdateValueOnce(T &variable, const T &value, bool &hasUpdatedValueOnce)
{
    if (!hasUpdatedValueOnce)
    {
        variable = value; // Update variable with value.
    }
    hasUpdatedValueOnce = true;
}

enum class CameraEdge
{
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

int GetCameraEdge(const raylib::Camera2D &camera, const raylib::Window &window, CameraEdge edge)
{
    switch (edge)
    {
        case CameraEdge::LEFT:
            return camera.target.x - (float)window.GetWidth() / 2 * camera.zoom;
        case CameraEdge::RIGHT:
            return camera.target.x + (float)window.GetWidth() / 2 * camera.zoom;
        case CameraEdge::TOP:
            return camera.target.y - (float)window.GetHeight() / 2 * camera.zoom;
        case CameraEdge::BOTTOM:
            return camera.target.y + (float)window.GetHeight() / 2 * camera.zoom;
        default:
            return camera.target.x;
    }
}

enum class CurrentScreen
{
    TITLE,
    SETTINGS,
    LEVEL_SELECTOR,
    IN_LEVEL,
};

float SetWindowFullScreen(raylib::Window *window, int winWidth, int winHeight, float delta)
{
    if (window->IsFullscreen())
    {
        window->ToggleFullscreen();
        window->SetSize(winWidth, winHeight);
        // return the camera zoom multiplier
        return (float)(winWidth) / (float)(GetMonitorWidth(MONITOR));
    }
    else
    {
        window->SetSize(GetMonitorWidth(MONITOR), GetMonitorHeight(MONITOR));
        WaitTime(delta);
        PollInputEvents();
        window->ToggleFullscreen();
        // return the camera zoom multiplier
        return 1.0f;
    }
}

int main()
{
    // Initialization
    int screenWidth = 1280;
    int screenHeight = 720;

    SetExitKey(KEY_NULL);
    raylib::Color textColor(BLACK);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    raylib::Window w(screenWidth, screenHeight, "Geometry Dash");
    raylib::AudioDevice audioDevice;

    raylib::Texture2D playerTextureCube("assets/player/cube.png");

    CurrentScreen currentScreen = CurrentScreen::TITLE;

    SetTargetFPS(FRAMERATE);
    SetWindowFullScreen(&w, screenWidth, screenHeight, 0.1);

    float winSizeZoomMultiplier = 1.0;

    // We use this camera to render zoom when changing the size of the window.
    // This camera is used when the playerCamera is not in use.
    // Otherwise, this is handled by playerCamera.
    raylib::Camera2D winSizeCamera(Vector2Zero(), Vector2Zero());

    Sprite logo(LoadTexture("assets/gui/logo.png"), raylib::Vector2{0.0f, 150.0f}, 1.0f);
    ParallaxSprite background(LoadTexture("assets/level/background1-3.png"), raylib::Vector2{0.0f, -500.0f}, 0.75f, 0.0f, raylib::Color{0x46a0ffff});
    background.hitbox.type = Hitbox::HitboxType::NONSOLID;
    ParallaxSprite ground(LoadTexture("assets/level/ground-long.png"), raylib::Vector2{0.0f, 900.0f}, 0.5f, 0.0f, raylib::Color{0x4a44ffff});
    ground.hitbox.type = Hitbox::HitboxType::SOLID;
    Sprite groundShadow(LoadTexture("assets/level/groundSquareShadow_001.png"), ground.position, 0.5f, 0.0f, raylib::Color{0xffffff88});
    Sprite groundLine(LoadTexture("assets/level/floorLine_001.png"), raylib::Vector2{0.0f, ground.position.y}, 0.75f, 0.0f, raylib::Color{0xffffffff});
    SpriteButton quitGameButton(LoadTexture("assets/gui/closeButton.png"));
    SpriteButton garageButton(LoadTexture("assets/gui/iconSelectorButton.png"));
    SpriteButton playButton(LoadTexture("assets/gui/levelSelectorButton.png"));
    SpriteButton createButton(LoadTexture("assets/gui/levelCreateButton.png"));

    bool hasUpdatedBackgroundPosY = false;

    quitGameButton.scale = 0.25f;
    quitGameButton.InitScale();

    Player player(LoadTexture("assets/player/cube.png"), raylib::Vector2{0.0f, 900.0f - 61.0f}, 0.5f);
    player.isPlatformer = false;

    PlayerCamera playerCamera(player);
    playerCamera.target = player.position;
    background.SetParallaxFactorX(0.75f);
    background.SetParallaxFactorY(0.5f);
    ground.SetParallaxFactorX(0.0f);
    player.position.y = -64.0f;

    FadeScreen fader(BLACK);
    raylib::Music menuLoop("assets/sounds/soundEffects/menuLoop.mp3");

    player.envItems.push_back(ground.hitbox);

    // Main game loop
    while (!w.ShouldClose())
    {
        // Time elapsed since the last frame
        float deltaTime = GetFrameTime() * player.time_scale;
        menuLoop.Update();

        if (quitGameButton.buttonEvent == SpriteButton::ButtonEvent::IN_ANIM_RELEASED &&
            quitGameButton.IsHovered(winSizeCamera) &&
            fader.fadeEvent != FadeScreen::FadeEvent::FADING_IN)
        {
            fader.FadeIn(0.25f);
        }
        if (quitGameButton.buttonEvent == SpriteButton::ButtonEvent::FINISHED_ANIM_RELEASED &&
            fader.fadeEvent == FadeScreen::FadeEvent::FADED_IN)
        {
            break;
        }

        if (playButton.buttonEvent == SpriteButton::ButtonEvent::IN_ANIM_RELEASED &&
            playButton.IsHovered(winSizeCamera) &&
            fader.fadeEvent != FadeScreen::FadeEvent::FADING_IN)
        {
            fader.FadeIn(0.25f);
        }
        if (playButton.buttonEvent == SpriteButton::ButtonEvent::FINISHED_ANIM_RELEASED &&
            fader.fadeEvent == FadeScreen::FadeEvent::FADED_IN)
        {
            currentScreen = CurrentScreen::IN_LEVEL;
            fader.FadeOut(0.0f);
            playButton.buttonEvent = SpriteButton::ButtonEvent::IDLE;
        }

        // TODO: Add a camera system.

        if (IsKeyDown(KEY_B))
        {
            playerCamera.zoom += deltaTime / player.time_scale * playerCamera.zoom;
        }
        if (IsKeyDown(KEY_N))
        {
            playerCamera.zoom -= deltaTime / player.time_scale * playerCamera.zoom;
        }
        if (IsKeyDown(KEY_C))
        {
            playerCamera.rotation += 1.0f;
        }

        switch (currentScreen)
        {
            case CurrentScreen::TITLE:
                if (!menuLoop.IsPlaying())
                {
                    menuLoop.Play();
                }
                background.position.x -= 250 * deltaTime;
                background.position.x = fmodf(background.position.x, background.texture.width * background.scale);
                ground.position.x -= 750 * deltaTime;
                ground.position.x = fmodf(ground.position.x, ground.texture.width * ground.scale);
                quitGameButton.RefreshButtonScale(deltaTime, winSizeCamera);
                garageButton.RefreshButtonScale(deltaTime, winSizeCamera);
                playButton.RefreshButtonScale(deltaTime, winSizeCamera);
                createButton.RefreshButtonScale(deltaTime, winSizeCamera);
                logo.position.x = logo.MoveToWinPercentage(raylib::Vector2{0.5f}).x;
                groundLine.position.x = groundLine.MoveToWinPercentage(raylib::Vector2{0.5f}).x;
                quitGameButton.position = quitGameButton.MoveToWinPercentage(raylib::Vector2{GetMonitorHeight(MONITOR) * 0.05f / GetMonitorWidth(MONITOR), 0.05f});
                garageButton.position = garageButton.MoveToWinPercentage(raylib::Vector2{0.35f, 0.5f});
                playButton.position = playButton.MoveToWinPercentage(raylib::Vector2{0.5f, 0.5f});
                createButton.position = createButton.MoveToWinPercentage(raylib::Vector2{0.65f, 0.5f});
                break;

            case CurrentScreen::IN_LEVEL:
                ground.position.y = 0.0f;
                // TODO Add a better condition to update the background vertical position for 1 tick.
                if (background.position.y != (ground.position.y - background.texture.height * background.scaleV.y) && !hasUpdatedBackgroundPosY)
                {
                    background.position.y = ground.position.y - background.texture.height * background.scaleV.y;
                }
                else
                {
                    hasUpdatedBackgroundPosY = true;
                }
                playerCamera.UpdateCamera();
                player.UpdatePlayer(deltaTime);
                background.UpdateParallax(playerCamera);
                ground.UpdateParallax(playerCamera);
                groundLine.position = raylib::Vector2{
                    playerCamera.GetCenter().x - (groundLine.texture.width * groundLine.scaleV.x) / 2,
                    ground.position.y};
                groundShadow.position = raylib::Vector2{
                    playerCamera.camera.target.x - playerCamera.camera.offset.x,
                    ground.position.y};
                menuLoop.Stop();
                break;
        }
        fader.RefreshFade(deltaTime);
        // Draw
        BeginDrawing();
        switch (currentScreen)
        {
            case CurrentScreen::TITLE:
                // BeginMode2D(winSizeCamera);
                ClearBackground(WHITE);
                background.Draw();
                background.Draw(raylib::Vector2{background.position.x + background.texture.width * background.scale});
                background.Draw(raylib::Vector2{background.position.x + background.texture.width * 2 * background.scale});
                ground.Draw();
                ground.Draw(raylib::Vector2{ground.position.x + ground.texture.width * ground.scale});
                groundShadow.Draw();
                groundShadow.Draw(
                    raylib::Vector2{
                        (float)(GetMonitorWidth(MONITOR)) - groundShadow.texture.width * groundShadow.scale,
                        ground.position.y},
                    180.0f);
                BeginBlendMode(BLEND_ADDITIVE);
                groundLine.Draw();
                EndBlendMode();
                logo.Draw();
                quitGameButton.Draw();
                garageButton.Draw();
                playButton.Draw();
                createButton.Draw();
                // EndMode2D();
                break;
            case CurrentScreen::IN_LEVEL:
                ClearBackground(WHITE);
                playerCamera.GetCamera().BeginMode();
                background.Draw(raylib::Vector2{background.position.x - background.texture.width * 2 * background.scaleV.x, background.position.y});
                background.Draw(raylib::Vector2{background.position.x - background.texture.width * background.scaleV.x, background.position.y});
                background.Draw();
                background.Draw(raylib::Vector2{background.position.x + background.texture.width * background.scaleV.x, background.position.y});
                background.Draw(raylib::Vector2{background.position.x + background.texture.width * 2 * background.scaleV.x, background.position.y});
                player.Draw();
                ground.Draw(raylib::Vector2{ground.position.x - ground.texture.width * ground.scaleV.x, ground.position.y});
                ground.Draw();
                ground.Draw(raylib::Vector2{ground.position.x + ground.texture.width * ground.scaleV.x, ground.position.y});
                groundShadow.Draw();
                groundShadow.Draw(
                    raylib::Vector2{
                        playerCamera.camera.target.x - playerCamera.camera.offset.x +
                            (float)(GetMonitorWidth(MONITOR)) - groundShadow.texture.width * groundShadow.scaleV.x,
                        ground.position.y},
                    180.0f);
                BeginBlendMode(BLEND_ADDITIVE);
                groundLine.Draw();
                EndBlendMode();
                playerCamera.GetCamera().EndMode();
                break;
            case CurrentScreen::LEVEL_SELECTOR:
                ClearBackground(WHITE);
                // BeginMode2D(winSizeCamera);
                DrawText("[LEVEL SELECTOR]", 1920 / 2 - 10 * 25, GetMonitorHeight(MONITOR) / 2 - 25, 50, BLACK);
                // EndMode2D();
                break;
            default:
                ClearBackground(WHITE);
                break;
        }
        fader.Draw();
        EndDrawing();
    }

    return 0;
}
