#include "fadescreen.hpp"
#include "hitbox.hpp"
#include "level.hpp"
#include "options.hpp"
#include "parallaxsprite.hpp"
#include "player.hpp"
#include "playercamera.hpp"
#include "sprite.hpp"
#include "spritebutton.hpp"
#include <Rectangle.hpp>
#include <Vector2.hpp>
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
    GARAGE_LEVEL_LIST
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

    Sprite logo("assets/gui/logo.png", raylib::Vector2{0.0f, 150.0f}, 1.0f);
    ParallaxSprite background(Sprite{"assets/level/background1-3.png", raylib::Vector2{0.0f, -500.0f}, 0.75f, 0.0f, raylib::Color{0x46a0ffff}});
    // background.sprite.texture = LoadTexture("assets/level/background1-3.png");
    background.sprite.hitbox.type = Hitbox::HitboxType::NONSOLID;
    ParallaxSprite ground(Sprite{"assets/level/ground-long.png", raylib::Vector2{0.0f, 900.0f}, 0.5f, 0.0f, raylib::Color{0x4a44ffff}});
    // ground.sprite.texture = LoadTexture("assets/level/ground-long.png");
    ground.sprite.hitbox.SetType(Hitbox::HitboxType::GROUND);
    Sprite groundShadow("assets/level/groundSquareShadow_001.png", ground.sprite.position, 0.5f, 0.0f, raylib::Color{0xffffff88});
    Sprite groundLine("assets/level/floorLine_001.png", raylib::Vector2{0.0f, ground.sprite.position.y}, 0.75f, 0.0f, raylib::Color{0xffffffff});
    SpriteButton quitGameButton(Sprite{"assets/gui/closeButton.png"});
    SpriteButton garageButton(Sprite{"assets/gui/iconSelectorButton.png"});
    SpriteButton playButton(Sprite{"assets/gui/levelSelectorButton.png"});
    SpriteButton createButton(Sprite{"assets/gui/levelCreateButton.png"});

    bool hasUpdatedBackgroundPosY = false;

    quitGameButton.sprite.scale = 0.25f;
    quitGameButton.InitScale();

    Player player(Sprite{"assets/player/cube.png", raylib::Vector2{0.0f, 900.0f - 61.0f}, 0.5f});
    player.isPlatformer = false;

    PlayerCamera playerCamera(player);
    playerCamera.target = player.icon.position;
    background.SetParallaxFactorX(0.75f);
    background.SetParallaxFactorY(0.5f);
    ground.SetParallaxFactorX(0.0f);
    player.icon.position.y = -64.0f;

    FadeScreen fader(BLACK);
    raylib::Music menuLoop("assets/sounds/soundEffects/menuLoop.mp3");

    Level testLevel;
    testLevel.song = raylib::Music("assets/sounds/levels/Secret Song.mp3");
    testLevel.objects.push_back(ground.sprite.hitbox);

    // Main game loop
    while (!w.ShouldClose())
    {
        // Time elapsed since the last frame
        float deltaTime = GetFrameTime() * player.time_scale;
        menuLoop.Update();

        if (quitGameButton.buttonEvent == SpriteButton::ButtonEvent::IN_ANIM_RELEASED &&
            quitGameButton.sprite.IsHovered(winSizeCamera) &&
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
            playButton.sprite.IsHovered(winSizeCamera) &&
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
                    testLevel.song.Stop();
                }
                background.sprite.position.x -= 250 * deltaTime;
                background.sprite.position.x = fmodf(background.sprite.position.x, background.sprite.texture.width * background.sprite.scale);
                ground.sprite.position.x -= 750 * deltaTime;
                ground.sprite.position.x = fmodf(ground.sprite.position.x, ground.sprite.texture.width * ground.sprite.scale);
                quitGameButton.RefreshButtonScale(deltaTime, winSizeCamera);
                garageButton.RefreshButtonScale(deltaTime, winSizeCamera);
                playButton.RefreshButtonScale(deltaTime, winSizeCamera);
                createButton.RefreshButtonScale(deltaTime, winSizeCamera);
                logo.position.x = logo.MoveToWinPercentage(raylib::Vector2{0.5f}).x;
                groundLine.position.x = groundLine.MoveToWinPercentage(raylib::Vector2{0.5f}).x;
                quitGameButton.sprite.position = quitGameButton.sprite.MoveToWinPercentage(raylib::Vector2{GetMonitorHeight(MONITOR) * 0.05f / GetMonitorWidth(MONITOR), 0.05f});
                garageButton.sprite.position = garageButton.sprite.MoveToWinPercentage(raylib::Vector2{0.35f, 0.5f});
                playButton.sprite.position = playButton.sprite.MoveToWinPercentage(raylib::Vector2{0.5f, 0.5f});
                createButton.sprite.position = createButton.sprite.MoveToWinPercentage(raylib::Vector2{0.65f, 0.5f});
                break;

            case CurrentScreen::IN_LEVEL:
                if (!testLevel.song.IsPlaying())
                {
                    testLevel.song.Play();
                    testLevel.song.Seek(testLevel.songStartTime);
                }
                ground.sprite.position.y = 0.0f;
                // TODO Add a better condition to update the background vertical position for 1 tick.
                if (background.sprite.position.y != (ground.sprite.position.y - background.sprite.texture.height * background.sprite.scaleV.y) && !hasUpdatedBackgroundPosY)
                {
                    background.sprite.position.y = ground.sprite.position.y - background.sprite.texture.height * background.sprite.scaleV.y;
                }
                else
                {
                    hasUpdatedBackgroundPosY = true;
                }
                playerCamera.UpdateCamera();
                player.UpdatePlayer(deltaTime, testLevel.objects);
                background.UpdateParallax(playerCamera);
                ground.UpdateParallax(playerCamera);
                groundLine.position = raylib::Vector2{
                    playerCamera.GetCameraCenter().x - (groundLine.texture.width * groundLine.scaleV.x) / 2,
                    ground.sprite.position.y};
                groundShadow.position = raylib::Vector2{
                    playerCamera.camera.target.x - playerCamera.camera.offset.x,
                    ground.sprite.position.y};
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
                background.sprite.Draw();
                background.sprite.Draw(raylib::Vector2{background.sprite.position.x + background.sprite.texture.width * background.sprite.scale});
                background.sprite.Draw(raylib::Vector2{background.sprite.position.x + background.sprite.texture.width * 2 * background.sprite.scale});
                ground.sprite.Draw();
                ground.sprite.Draw(raylib::Vector2{ground.sprite.position.x + ground.sprite.texture.width * ground.sprite.scale});
                groundShadow.Draw();
                groundShadow.Draw(
                    raylib::Vector2{
                        (float)(GetMonitorWidth(MONITOR)) - groundShadow.texture.width * groundShadow.scale,
                        ground.sprite.position.y},
                    180.0f);
                BeginBlendMode(BLEND_ADDITIVE);
                groundLine.Draw();
                EndBlendMode();
                logo.Draw();
                quitGameButton.sprite.Draw();
                garageButton.sprite.Draw();
                playButton.sprite.Draw();
                createButton.sprite.Draw();
                // EndMode2D();
                break;
            case CurrentScreen::IN_LEVEL:
                ClearBackground(WHITE);
                playerCamera.GetCamera().BeginMode();
                background.sprite.Draw(raylib::Vector2{background.sprite.position.x - background.sprite.texture.width * 2 * background.sprite.scaleV.x, background.sprite.position.y});
                background.sprite.Draw(raylib::Vector2{background.sprite.position.x - background.sprite.texture.width * background.sprite.scaleV.x, background.sprite.position.y});
                background.sprite.Draw();
                background.sprite.Draw(raylib::Vector2{background.sprite.position.x + background.sprite.texture.width * background.sprite.scaleV.x, background.sprite.position.y});
                background.sprite.Draw(raylib::Vector2{background.sprite.position.x + background.sprite.texture.width * 2 * background.sprite.scaleV.x, background.sprite.position.y});
                player.icon.Draw();
                ground.sprite.Draw(raylib::Vector2{ground.sprite.position.x - ground.sprite.texture.width * ground.sprite.scaleV.x, ground.sprite.position.y});
                ground.sprite.Draw();
                ground.sprite.Draw(raylib::Vector2{ground.sprite.position.x + ground.sprite.texture.width * ground.sprite.scaleV.x, ground.sprite.position.y});
                groundShadow.Draw();
                groundShadow.Draw(
                    raylib::Vector2{
                        playerCamera.camera.target.x - playerCamera.camera.offset.x +
                            (float)(GetMonitorWidth(MONITOR)) - groundShadow.texture.width * groundShadow.scaleV.x,
                        ground.sprite.position.y},
                    180.0f);
                BeginBlendMode(BLEND_ADDITIVE);
                groundLine.Draw();
                EndBlendMode();
                // player.bigHitbox.Draw(RED);
                // player.smallHitbox.Draw(BLUE);
                playerCamera.GetCamera()
                    .EndMode();
                break;
            case CurrentScreen::LEVEL_SELECTOR:
                ClearBackground(WHITE);
                // BeginMode2D(winSizeCamera);
                DrawText("[LEVEL SELECTOR]", 1920 / 2 - 10 * 25, GetMonitorHeight(MONITOR) / 2 - 25, 50, BLACK);
                // EndMode2D();
                break;
            case CurrentScreen::GARAGE_LEVEL_LIST:
                ClearBackground(WHITE);
                // BeginMode2D(winSizeCamera);
                DrawText("[GARAGE LEVEL LIST]", 1920 / 2 - 10 * 25, GetMonitorHeight(MONITOR) / 2 - 25, 50, BLACK);
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
