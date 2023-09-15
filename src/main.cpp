#include <iostream>
#include <raylib-cpp.hpp>
#include "reasings.h"

#define FRAMERATE 60
#define MONITOR GetCurrentMonitor()
#define SHOW_BOXES false

class Sprite
{
public:
	raylib::Texture2D texture;
	raylib::Rectangle debugRect;
	raylib::Color rectColor;
	raylib::Vector2 position;
	float scale;			// Scale as a float (equal X and Y scale).
	raylib::Vector2 scaleV; // Scale as a raylib::Vector2.
	float rotation;
	raylib::Color tint;
	raylib::Vector2 flip = raylib::Vector2{1.0f, 1.0f};

	Sprite(Texture2D self_texture, raylib::Vector2 self_position = raylib::Vector2{0.0, 0.0}, float self_scale = 1.0f, float self_rotation = 0.0f, raylib::Color self_tint = WHITE)
		: texture(self_texture), position(self_position), scale(self_scale), rotation(self_rotation), tint(self_tint)
	{
		scaleV = raylib::Vector2{scale, scale};
		texture.SetFilter(TEXTURE_FILTER_TRILINEAR);
		rectColor = raylib::Color((GetRandomValue(0, 255)), (GetRandomValue(0, 255)), (GetRandomValue(0, 255)), 50);
		// rectColor = raylib::Color(0xff000088);
	};
	void Draw()
	{
		scaleV = raylib::Vector2{scale, scale};
		debugRect.SetPosition(position);
		debugRect.SetWidth(texture.GetWidth() * abs(scale));
		debugRect.SetHeight(texture.GetHeight() * abs(scale));
		if (scale < 0)
		{
			debugRect.x -= texture.width * abs(scale);
			debugRect.y -= texture.height * abs(scale);
		}
		texture.Draw(
			// sourceRec
			raylib::Rectangle{
				0.0f, 0.0f,
				(float)texture.GetWidth() * flip.x,
				(float)texture.GetHeight() * flip.y},
			// destRec
			raylib::Rectangle{
				position.x + (float)texture.GetWidth() / 2 * scaleV.x,
				position.y + (float)texture.GetHeight() / 2 * scaleV.y,
				scaleV.x * (float)texture.GetWidth(),
				scaleV.y * (float)texture.GetHeight()},
			// origin x&y
			raylib::Vector2{
				(float)texture.GetWidth() / 2 * scaleV.x,
				(float)texture.GetHeight() / 2 * scaleV.y},
			rotation, tint);
		if (SHOW_BOXES)
		{
			debugRect.Draw(rectColor);
		}
	};
	void Draw(raylib::Vector2 drawPosition)
	{
		if (drawPosition.y == 0 && position.y != 0.0)
		{
			drawPosition.y = position.y;
		}
		scaleV = raylib::Vector2{scale, scale};
		debugRect.SetPosition(drawPosition);
		debugRect.SetWidth(texture.GetWidth() * abs(scale));
		debugRect.SetHeight(texture.GetHeight() * abs(scale));
		if (scale < 0)
		{
			debugRect.x -= texture.width * abs(scale);
			debugRect.y -= texture.height * abs(scale);
		}
		texture.Draw(
			// sourceRec
			raylib::Rectangle{
				0.0f, 0.0f,
				(float)texture.GetWidth() * flip.x,
				(float)texture.GetHeight() * flip.y},
			// destRec
			raylib::Rectangle{
				drawPosition.x + (float)texture.GetWidth() / 2 * scaleV.x,
				drawPosition.y + (float)texture.GetHeight() / 2 * scaleV.y,
				scaleV.x * (float)texture.GetWidth(),
				scaleV.y * (float)texture.GetHeight()},
			// origin x&y
			raylib::Vector2{
				(float)texture.GetWidth() / 2 * scaleV.x,
				(float)texture.GetHeight() / 2 * scaleV.y},
			rotation, tint);
		if (SHOW_BOXES)
		{
			debugRect.Draw(rectColor);
		}
	};
	void Draw(raylib::Vector2 drawPosition, float drawRotation)
	{
		if (drawPosition.y == 0 && position.y != 0.0)
		{
			drawPosition.y = position.y;
		}
		scaleV = raylib::Vector2{scale, scale};
		debugRect.SetPosition(drawPosition);
		debugRect.SetWidth(texture.GetWidth() * abs(scale));
		debugRect.SetHeight(texture.GetHeight() * abs(scale));
		if (scale < 0)
		{
			debugRect.x -= texture.width * abs(scale);
			debugRect.y -= texture.height * abs(scale);
		}
		texture.Draw(
			// sourceRec
			raylib::Rectangle{
				0.0f, 0.0f,
				(float)texture.GetWidth() * flip.x,
				(float)texture.GetHeight() * flip.y},
			// destRec
			raylib::Rectangle{
				drawPosition.x + ((float)texture.GetWidth() / 2) * scaleV.x,
				drawPosition.y + ((float)texture.GetHeight() / 2) * scaleV.y,
				scaleV.x * (float)texture.GetWidth(),
				scaleV.y * (float)texture.GetHeight()},
			// origin x&y
			raylib::Vector2{
				(float)texture.GetWidth() / 2 * scaleV.x,
				(float)texture.GetHeight() / 2 * scaleV.y},
			drawRotation, tint);
		if (SHOW_BOXES)
		{
			debugRect.Draw(rectColor);
		}
	};
	bool IsHovered(raylib::Camera2D &camera)
	{
		raylib::Rectangle translatedRect = raylib::Rectangle{
			GetWorldToScreen2D(position, camera).x,
			GetWorldToScreen2D(position, camera).y,
			static_cast<float>(texture.width) * camera.GetZoom(),
			static_cast<float>(texture.height) * camera.GetZoom()};
		if (translatedRect.CheckCollision(GetMousePosition()))
		{
			return true;
		}
		else
		{
			return false;
		}
	};
	raylib::Vector2 MoveToWinPercentage(raylib::Vector2 percentage);
	raylib::Vector2 MoveToWinPercentage(raylib::Vector2 percentage, raylib::Rectangle rec);
	raylib::Vector2 GetWinPercentage();
	raylib::Vector2 GetWinPercentage(raylib::Rectangle rec);
	raylib::Vector2 GetCenter();
};

raylib::Vector2 Sprite::GetCenter()
{
	return raylib::Vector2{
		position.x + static_cast<float>(texture.width) * scale * 0.5f,
		position.y + static_cast<float>(texture.height) * scale * 0.5f};
}

// Get percentage of the width and height of the game window from the sprite's position, width, height and scale.
raylib::Vector2 Sprite::GetWinPercentage()
{
	return raylib::Vector2{
		GetCenter().x / GetMonitorWidth(MONITOR),
		GetCenter().y / GetMonitorHeight(MONITOR)};
}

// Get percentage of the width and height of the game window from the sprite's position, width, height and scale.
raylib::Vector2 Sprite::GetWinPercentage(raylib::Rectangle rec)
{
	return raylib::Vector2{
		GetCenter().x / rec.width,
		GetCenter().y / rec.height};
}

// Move Sprite to percentage of the width and height of the game window.
raylib::Vector2 Sprite::MoveToWinPercentage(raylib::Vector2 percentage)
{
	return raylib::Vector2{
		(GetMonitorWidth(MONITOR) * percentage.x) - (texture.width / 2) * scale,
		(GetMonitorHeight(MONITOR) * percentage.y) - (texture.height / 2) * scale};
}

// Move Sprite to percentage of the width and height of the specified Rectangle.
raylib::Vector2 Sprite::MoveToWinPercentage(raylib::Vector2 percentage, raylib::Rectangle rec)
{
	return raylib::Vector2{
		(rec.width * percentage.x) - (texture.width / 2) * scale,
		(rec.height * percentage.y) - (texture.height / 2) * scale};
}

enum class Edge
{
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

int GetEdge(const raylib::Camera2D &camera, const raylib::Window &window, Edge edge)
{
	switch (edge)
	{
	case Edge::LEFT:
		return camera.target.x - (float)window.GetWidth() / 2 * camera.zoom;
	case Edge::RIGHT:
		return camera.target.x + (float)window.GetWidth() / 2 * camera.zoom;
	case Edge::TOP:
		return camera.target.y - (float)window.GetHeight() / 2 * camera.zoom;
	case Edge::BOTTOM:
		return camera.target.y + (float)window.GetHeight() / 2 * camera.zoom;
	default:
		return camera.target.x;
	}
}

class ParallaxSprite : public Sprite
{
private:
	float parallaxFactor = 0.0f;
	float previousCameraX = 0.0f;
	float deltaCameraX = 0.0f;

public:
	using Sprite::Sprite;
	void SetParallaxFactor(float factor) { parallaxFactor = factor; };
	float GetParallaxFactor() { return parallaxFactor; };
	void UpdateParallax(raylib::Camera2D &camera)
	{
		// Actual parallax code
		if (parallaxFactor != 0.0f)
		{
			position.x += deltaCameraX * parallaxFactor;
			deltaCameraX = camera.target.x - previousCameraX;
			previousCameraX = camera.target.x;
		}

		// Scroll the background if it overflows, to give the impression of an infinite background.
		if ((position.x + texture.width * scaleV.x) < camera.GetScreenToWorld(Vector2Zero()).x)
		{
			position.x += (texture.width * scaleV.x);
		}
		if (position.x > camera.GetScreenToWorld(raylib::Vector2{(float)GetMonitorWidth(MONITOR), 0.0f}).x)
		{
			std::cout << "1: " << position.x << std::endl;
			position.x -= (texture.width * scaleV.x);
			std::cout << "2: " << position.x << std::endl;
		}
	};
};

class Player : public Sprite
{
public:
	raylib::Vector2 speed{625.0, 20.0};
	raylib::Vector2 velocity;
	bool isPlatformer = true;
	raylib::Vector2 direction;
	int horizontal_direction = 1; // -1 = left, 1 = right
	int jump_direction;
	const float gravity = 5000;
	const float friction = 50;
	using Sprite::Sprite;
	void UpdatePlayer(float delta);
};

void Player::UpdatePlayer(float delta)
{
	// Get direction
	if (isPlatformer)
	{
		direction.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
		horizontal_direction = direction.x;
	}
	else
	{
		direction.x = horizontal_direction;
	}

	// Friction calculation

	if (direction.x != 0.0)
	{
		velocity.x = direction.x * speed.x * delta;
		flip.x = direction.x;
	}
	if (direction.x == 0)
	{
		// Threshold is set to 1 pixel because there are no sub-pixel movements
		if (velocity.x > 1)
		{
			velocity.x -= friction * delta;
		}
		else if (velocity.x < -1)
		{
			velocity.x += friction * delta;
		}
		else
		{
			velocity.x = 0;
		}
	}
	position.x += velocity.x;
};

class SpriteButton : public Sprite
{
private:
	float scaleDefault = 1.0f;
	float scaleTarget = 1.2f;	// Target scale when the button is pressed
	float scaleDuration = 0.2f; // Duration of the scale animation
	float elapsedTime;

public:
	using Sprite::Sprite;
	void ResetElapsedTime() { elapsedTime = 0.0f; };
	void InitScale()
	{
		scaleDefault *= scale;
		scaleTarget *= scale;
	}
	enum class ButtonEvent
	{
		IDLE,				   // Only used when initializing the ButtonEvent to avoid triggering a press or release.
		IN_ANIM_PRESSED,	   // Is currently in the press animation.
		IN_ANIM_RELEASED,	   // Is currently in the release animation.
		FINISHED_ANIM_PRESSED, // Has finished the press animation (idle).
		FINISHED_ANIM_RELEASED // Has finished the release animation (idle).
	};
	ButtonEvent buttonEvent;
	void RefreshButtonScale(float delta, raylib::Camera2D &camera)
	{
		if (IsHovered(camera) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
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
			if (scale > scaleDefault)
			{
				scale = EaseBounceOut(elapsedTime, scaleTarget, scaleDefault - scaleTarget, scaleDuration);
				elapsedTime += delta;
			}
			else
			{
				buttonEvent = ButtonEvent::FINISHED_ANIM_RELEASED;
			}
		}
		else if (buttonEvent == ButtonEvent::IN_ANIM_PRESSED)
		{
			if (scale < scaleTarget)
			{
				scale = EaseBounceOut(elapsedTime, scaleDefault, scaleTarget - scaleDefault, scaleDuration);
				elapsedTime += delta;
			}
			else
			{
				buttonEvent = ButtonEvent::FINISHED_ANIM_PRESSED;
			}
		}
	}
};

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
		IDLE,		// Only used when initializing the FadeEvent to avoid triggering a fade-in or fade-out.
		FADING_IN,	// Is currently fading in.
		FADING_OUT, // Is currently fading out.
		FADED_IN,	// Has finished fading in (idle).
		FADED_OUT	// Has finished fading out (idle).
	};
	FadeEvent fadeEvent;
	FadeScreen(raylib::Color self_color)
		: color(self_color)
	{
		rec.SetPosition(0.0f, 0.0f);
		rec.SetSize(GetMonitorWidth(MONITOR), GetMonitorHeight(MONITOR));
	};
	void Draw()
	{
		color.a = alpha;
		rec.Draw(color);
	}
	void FadeIn(float duration)
	{
		fadeEvent = FadeEvent::FADING_IN;
		fadeDuration = duration;
		elapsedTime = 0.0f;
	};
	void FadeOut(float duration)
	{
		fadeEvent = FadeEvent::FADING_OUT;
		fadeDuration = duration;
		elapsedTime = 0.0f;
	};
	void RefreshFade(float delta)
	{
		if (fadeEvent == FadeEvent::FADING_OUT)
		{
			if (alpha > alphaDefault)
			{
				alpha = EaseSineIn(elapsedTime, alphaTarget, alphaDefault - alphaTarget, fadeDuration);
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
				alpha = EaseSineIn(elapsedTime, alphaDefault, alphaTarget - alphaDefault, fadeDuration);
				elapsedTime += delta;
			}
			else
			{
				fadeEvent = FadeEvent::FADED_IN;
			}
		}
	}
};

class PlayerCamera
{
private:
	const float MAX_HEIGHT = 225.0f;
	const float MIN_HEIGHT = -5000.0f;
	const float MAX_DIST = 200.0f; // Max vertical distance from the player.
	float dist;					   // Camera's vertical distance from the player.
	bool freefly = true;

public:
	raylib::Camera2D camera = raylib::Camera2D{Vector2Zero(), Vector2Zero()};
	raylib::Camera2D GetCamera() { return camera; }
	// TODO Will need to add a bool to determine if the target is the player or a static position.
	raylib::Vector2 target;
	raylib::Vector2 offset;
	bool is_static_x = false;
	bool is_static_y = false;
	Player &player;
	float zoom;
	float rotation; // in degrees
	PlayerCamera(Player &self_player)
		: player(self_player){};
	raylib::Vector2 GetCenter()
	{
		return raylib::Vector2{
			camera.target.x - camera.offset.x + GetMonitorWidth(MONITOR) / 2,
			camera.target.y - camera.offset.y + GetMonitorHeight(MONITOR) / 2,
		};
	}
	void UpdateCamera()
	{
		// TODO Implement proper camera lerping and static
		dist = player.GetCenter().y - GetCenter().y;
		if (!is_static_x)
		{
			camera.target.x = Lerp(
				camera.target.x,
				player.GetCenter().x - (float)GetMonitorWidth(MONITOR) / 2,
				0.5f);

			if (player.isPlatformer)
			{
				camera.offset = Vector2Lerp(
					camera.offset,
					raylib::Vector2{90.0f * (float)player.horizontal_direction, 0.0f},
					0.1f);
			}
			else
			{
				camera.offset = Vector2Lerp(
					camera.offset,
					raylib::Vector2{-300.0f * (float)player.horizontal_direction, 0.0f},
					0.05f);
			}
		}
		if (!is_static_y && freefly && abs(dist) < MAX_DIST)
		{
			camera.target.y = Lerp(
				camera.target.y,
				Clamp(player.GetCenter().y - copysignf(1.0, dist), MIN_HEIGHT, MAX_HEIGHT),
				0.1f);
		}
	};
};

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
	ParallaxSprite ground(LoadTexture("assets/level/ground-long.png"), raylib::Vector2{0.0f, 900.0f}, 0.5f, 0.0f, raylib::Color{0x4a44ffff});
	Sprite groundShadow(LoadTexture("assets/level/groundSquareShadow_001.png"), ground.position, 0.5f, 0.0f, raylib::Color{0xffffff88});
	Sprite groundLine(LoadTexture("assets/level/floorLine_001.png"), raylib::Vector2{0.0f, ground.position.y}, 0.75f, 0.0f, raylib::Color{0xffffffff});
	SpriteButton quitGameButton(LoadTexture("assets/gui/closeButton.png"));
	SpriteButton garageButton(LoadTexture("assets/gui/iconSelectorButton.png"));
	SpriteButton playButton(LoadTexture("assets/gui/levelSelectorButton.png"));
	SpriteButton createButton(LoadTexture("assets/gui/levelCreateButton.png"));

	quitGameButton.scale = 0.25f;
	quitGameButton.InitScale();

	Player player(LoadTexture("assets/player/cube.png"), raylib::Vector2{0.0f, 900.0f - 61.0f}, 0.5f);

	PlayerCamera playerCamera(player);
	playerCamera.target = player.position;

	background.SetParallaxFactor(0.75f);
	ground.SetParallaxFactor(0.0f);

	FadeScreen fader(BLACK);
	raylib::Music menuLoop("assets/sounds/soundEffects/menuLoop.mp3");

	// Main game loop
	while (!w.ShouldClose())
	{
		// Time elapsed since the last frame
		float deltaTime = GetFrameTime();
		menuLoop.Update();

		if (quitGameButton.buttonEvent == SpriteButton::ButtonEvent::IN_ANIM_RELEASED && quitGameButton.IsHovered(winSizeCamera) && fader.fadeEvent != FadeScreen::FadeEvent::FADING_IN)
		{
			fader.FadeIn(0.25f);
		}
		if (quitGameButton.buttonEvent == SpriteButton::ButtonEvent::FINISHED_ANIM_RELEASED && fader.fadeEvent == FadeScreen::FadeEvent::FADED_IN)
		{
			break;
		}

		if (playButton.buttonEvent == SpriteButton::ButtonEvent::IN_ANIM_RELEASED && playButton.IsHovered(winSizeCamera) && fader.fadeEvent != FadeScreen::FadeEvent::FADING_IN)
		{
			fader.FadeIn(0.25f);
		}
		if (playButton.buttonEvent == SpriteButton::ButtonEvent::FINISHED_ANIM_RELEASED && fader.fadeEvent == FadeScreen::FadeEvent::FADED_IN)
		{
			currentScreen = CurrentScreen::IN_LEVEL;
			fader.FadeOut(0.25f);
			playButton.buttonEvent = SpriteButton::ButtonEvent::IDLE;
		}
		

		// TODO: Add a camera system.

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
			player.UpdatePlayer(deltaTime);
			playerCamera.UpdateCamera();
			background.UpdateParallax(playerCamera.camera);
			ground.UpdateParallax(playerCamera.camera);
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
					playerCamera.camera.target.x - playerCamera.camera.offset.x + (float)(GetMonitorWidth(MONITOR)) - groundShadow.texture.width * groundShadow.scaleV.x,
					ground.position.y},
				180.0f);
			groundLine.Draw();
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
