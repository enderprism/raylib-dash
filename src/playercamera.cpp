#include "playercamera.hpp"
#include "options.hpp"
#include <raylib-cpp.hpp>

PlayerCamera::PlayerCamera(Player &self_player)
    : player(self_player)
{
    camera.target = player.icon.GetCenter();
    if (player.isPlatformer)
    {
        camera.offset = raylib::Vector2{
            ((float)GetMonitorWidth(MONITOR) / 2) + 90.0f * (float)player.horizontal_direction,
            ((float)GetMonitorHeight(MONITOR) / 2)};
    }
    else
    {
        camera.offset = raylib::Vector2{((float)GetMonitorWidth(MONITOR) / 2) + (-300.0f * (float)player.horizontal_direction),
                                        ((float)GetMonitorHeight(MONITOR) / 2)};
    }
};

raylib::Vector2 PlayerCamera::GetCameraCenter()
{
    camera.rotation = 0.0f;
    raylib::Vector2 result = camera.GetScreenToWorld(raylib::Vector2{
        (float)GetMonitorWidth(MONITOR) / 2,
        (float)GetMonitorHeight(MONITOR) / 2,
    });
    camera.rotation = rotation;
    return result;
}

void PlayerCamera::UpdateCamera()
{
    // Set previous values to compute deltas
    prevTarget = target + camera.target;
    prevOffset = offset + camera.offset;

    camera.zoom = zoom;
    camera.rotation = rotation;
    // TODO Implement proper camera lerping and static
    dist = player.icon.GetCenter().y - GetCameraCenter().y;
    if (!is_static_x)
    {
        // camera.target = Vector2Lerp(
        // 	camera.target,
        // 	player.GetCenter(),
        // 	0.5f);
        camera.target.x = Lerp(camera.target.x, player.icon.GetCenter().x, 0.5f);
        if (player.isPlatformer)
        {
            camera.offset = Vector2Lerp(
                camera.offset,
                raylib::Vector2{
                    ((float)GetMonitorWidth(MONITOR) / 2) + 90.0f * (float)player.horizontal_direction,
                    ((float)GetMonitorHeight(MONITOR) / 2)},
                0.05f);
            // camera.offset.x = Lerp(
            // 	camera.offset.x,
            // 	(float)(GetMonitorWidth(MONITOR) / 2) + 90.0f * (float)player.horizontal_direction,
            // 	0.05f);
        }
        else
        {
            camera.offset = Vector2Lerp(
                camera.offset,
                raylib::Vector2{
                    ((float)GetMonitorWidth(MONITOR) / 2) + (-300.0f * (float)player.horizontal_direction),
                    ((float)GetMonitorHeight(MONITOR) / 2)},
                0.1f);
        }
    }

    if (!is_static_y && freefly && abs(dist) > MAX_DIST)
    {
        // FIXME Fix the camera dist y offset thing
        camera.target.y = Lerp(
            camera.target.y,
            player.icon.GetCenter().y,
            0.03f);
    }
    camera.rotation = 0.0f;
    if (camera.GetScreenToWorld(raylib::Vector2{0.0f, (float)GetMonitorHeight(MONITOR)}).y > GROUND_HEIGHT)
    {
        camera.target.y -= camera.GetScreenToWorld(raylib::Vector2{0.0f, (float)GetMonitorHeight(MONITOR)}).y - GROUND_HEIGHT;
    }
    camera.rotation = rotation;

    // Set deltas for ParallaxSprite
    deltaTarget = (target + camera.target) - prevTarget;
    deltaOffset = (offset + camera.offset) - prevOffset;
};
