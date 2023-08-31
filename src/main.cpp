#include <raylib-cpp.hpp>
#include <iostream>
#include <reasings.h>

#define FRAMERATE 60
#define SHOW_BOXES false
#define VECTOR2_ZERO raylib::Vector2{0.0f, 0.0f}

class Sprite
{
public:
	raylib::Texture2D texture;
	raylib::Rectangle rect;
	raylib::Color rectColor;
	raylib::Vector2 position;
	float scale;
	float rotation;
	raylib::Color tint; // Add a member for tint

	Sprite(Texture2D self_texture, raylib::Vector2 self_position = raylib::Vector2{0.0, 0.0}, float self_scale = 1.0f, float self_rotation = 0.0f, raylib::Color self_tint = WHITE)
		: texture(self_texture), position(self_position), scale(self_scale), rotation(self_rotation), tint(self_tint)
	{
		texture.SetFilter(TEXTURE_FILTER_TRILINEAR);
		rectColor = raylib::Color((GetRandomValue(0, 255)), (GetRandomValue(0, 255)), (GetRandomValue(0, 255)), 50);
		// rectColor = raylib::Color(0xff000088);
	};
	void Draw() {
		rect.SetPosition(position);
		rect.SetWidth(texture.GetWidth()*abs(scale));
		rect.SetHeight(texture.GetHeight()*abs(scale));
		if (scale < 0)
		{
			rect.x -= texture.width*abs(scale);
			rect.y -= texture.height*abs(scale);
		}
		texture.Draw(position, rotation, scale, tint);
		if (SHOW_BOXES) { rect.Draw(rectColor); }
	};
	void Draw(raylib::Vector2 drawPosition) {
		if (drawPosition.y == 0 && position.y != 0.0) {
			drawPosition.y = position.y;
		}
		rect.SetPosition(position);
		rect.SetWidth(texture.GetWidth()*scale);
		rect.SetHeight(texture.GetHeight()*scale);
		texture.Draw(drawPosition, rotation, scale, tint);
		if (SHOW_BOXES) { rect.Draw(rectColor); }
	};
	void Draw(raylib::Vector2 drawPosition, float drawRotation) {
		if (drawPosition.y == 0 && position.y != 0.0) {
			drawPosition.y = position.y;
		}
		rect.SetPosition(position);
		rect.SetWidth(texture.GetWidth()*scale);
		rect.SetHeight(texture.GetHeight()*scale);
		texture.Draw(drawPosition, drawRotation, scale, tint);
		if (SHOW_BOXES) { rect.Draw(rectColor); }
	};
	bool IsHovered(raylib::Camera2D& camera) {
		raylib::Rectangle translatedRect = raylib::Rectangle{
			GetWorldToScreen2D(position, camera).x,
			GetWorldToScreen2D(position, camera).y,
			static_cast<float>(texture.width) * camera.GetZoom(),
			static_cast<float>(texture.height) * camera.GetZoom()
		};
		if (translatedRect.CheckCollision(GetMousePosition())) {
			return true;
		} else {
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
	return raylib::Vector2
	{
		position.x + static_cast<float>(texture.width) * scale * 0.5f,
		position.y + static_cast<float>(texture.height) * scale * 0.5f
	};
}

// Get percentage of the width and height of the game window from the sprite's position, width, height and scale.
raylib::Vector2 Sprite::GetWinPercentage()
{
	int monitor = GetCurrentMonitor();
	return raylib::Vector2
	{
		GetCenter().x/GetMonitorWidth(monitor),
		GetCenter().y/GetMonitorHeight(monitor)
	};
}

// Get percentage of the width and height of the game window from the sprite's position, width, height and scale.
raylib::Vector2 Sprite::GetWinPercentage(raylib::Rectangle rec)
{
	return raylib::Vector2
	{
		GetCenter().x/rec.width,
		GetCenter().y/rec.height
	};
}

// Move Sprite to percentage of the width and height of the game window.
raylib::Vector2 Sprite::MoveToWinPercentage(raylib::Vector2 percentage) {
	int monitor = GetCurrentMonitor();
	return raylib::Vector2
	{
		(GetMonitorWidth(monitor)*percentage.x)-(texture.width/2)*scale,
		(GetMonitorHeight(monitor)*percentage.y)-(texture.height/2)*scale
	};
}

// Move Sprite to percentage of the width and height of the specified Rectangle.
raylib::Vector2 Sprite::MoveToWinPercentage(raylib::Vector2 percentage, raylib::Rectangle rec) {
	return raylib::Vector2
	{
		(rec.width*percentage.x)-(texture.width/2)*scale,
		(rec.height*percentage.y)-(texture.height/2)*scale
	};
}

enum class Edge {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

int GetEdge(const raylib::Camera2D& camera, const raylib::Window& window, Edge edge)
{
	switch (edge)
	{
		case Edge::LEFT: return camera.target.x - window.GetWidth()/2 * camera.zoom;
		case Edge::RIGHT: return camera.target.x + window.GetWidth()/2 * camera.zoom;
		case Edge::TOP: return camera.target.y - window.GetHeight()/2 * camera.zoom;
		case Edge::BOTTOM: return camera.target.y + window.GetHeight()/2 * camera.zoom;
		default: return camera.target.x;
	}
}

class ParallaxSprite: public Sprite 
{
public:
	using Sprite::Sprite;
	void SetParallaxFactor(float factor) { parallaxFactor = factor; };
	float GetParallaxFactor() { return parallaxFactor; };
private:
	float parallaxFactor;
};

class Player: public Sprite
{
public:
	raylib::Vector2 speed{625.0, 20.0};
	raylib::Vector2 velocity;
	bool isPlatformer = true;
	raylib::Vector2 direction;
	const float gravity = 5000;
	const float friction = 625;
	using Sprite::Sprite;
	void UpdatePlayer(float delta);
};

void Player::UpdatePlayer(float delta) {
	// Get direction

	direction.x = IsKeyDown(KEY_RIGHT)-IsKeyDown(KEY_LEFT);

	// Friction calculation

	if (direction.x != 0.0) {
		velocity.x = direction.x * speed.x * delta;
	}
	if (direction.x == 0) {
		// Threshold is set to 1 pixel because there are no sub-pixel movements
		if (velocity.x > 1) {
			velocity.x -= friction * delta;
		} else if (velocity.x < -1) {
			velocity.x += friction * delta;
		} else {
			velocity.x = 0;
		}
	}
	position.x += velocity.x;

};

class SpriteButton: public Sprite
{
private:
	float scaleDefault = 1.0f;
	float scaleTarget = 1.2f; // Target scale when the button is pressed
	float scaleDuration = 0.2f; // Duration of the scale animation
	double elapsedTime;
public:
	using Sprite::Sprite;
	bool hasBeenPressed = false;
	bool hasBeenReleased = false;
	void RefreshButtonScale(float delta, raylib::Camera2D& camera) {
		if (IsHovered(camera) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			hasBeenPressed = true;
			hasBeenReleased = false;
			elapsedTime = 0.0;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && hasBeenPressed)
		{
			hasBeenPressed = false;
			hasBeenReleased = true;
			elapsedTime = 0.0;
		}
		if (hasBeenReleased)
		{
			if (scale > scaleDefault)
			{
				scale = EaseBounceOut(elapsedTime, scaleTarget, scaleDefault - scaleTarget, scaleDuration);
				elapsedTime += delta;
			}
			else
			{
				hasBeenReleased = false;
			}
		}
		else if (hasBeenPressed)
		{
			if (scale < scaleTarget)
			{
				scale = EaseBounceOut(elapsedTime, scaleDefault, scaleTarget - scaleDefault, scaleDuration);
				elapsedTime += delta;
			}
		}
	}
};

enum class CurrentScreen {
	TITLE,
	SETTINGS,
	LEVEL_SELECTOR,
	IN_LEVEL,
};

float SetWindowFullScreen(raylib::Window *window, int winWidth, int winHeight, float delta)
{
	int monitor = GetCurrentMonitor();
	if (window->IsFullscreen()) {
		window->ToggleFullscreen();
		window->SetSize(winWidth, winHeight);
		// return the camera zoom multiplier
		return static_cast<float>(winWidth)/static_cast<float>(GetMonitorWidth(monitor));
	} else {
		window->SetSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		WaitTime(delta);
		PollInputEvents();
		window->ToggleFullscreen();
		// return the camera zoom multiplier
		return 1.0f;
	}
}

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
	enum class FadeEvent
	{
		IDLE, // Only used when initializing the FadeEvent to avoid triggering a fade-in or fade-out.
		FADING_IN, // Is currently fading in.
		FADING_OUT, // Is currently fading in.
		FADED_IN, // Has finished fading in (idle).
		FADED_OUT // Has finished fading out (idle).
	};
	FadeEvent fadeEvent;
	bool hasStartedFadingIn = false;
	bool hasStartedFadingOut = false;
	FadeScreen(raylib::Color self_color) : color(self_color)
	{
		int monitor = GetCurrentMonitor();
		rec.SetPosition(0.0f, 0.0f);
		rec.SetSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
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
				alpha = EaseSineOut(elapsedTime, alphaTarget, alphaDefault - alphaTarget, fadeDuration);
				elapsedTime += delta;
			}
			else
			{
				fadeEvent = FadeEvent::FADED_OUT;
			}
		}
		else if (fadeEvent == FadeEvent::FADING_IN)
		{
			if (alpha < 255)
			{
				alpha = EaseSineIn(elapsedTime, alphaDefault, alphaTarget - alphaDefault, fadeDuration);
				elapsedTime += delta;
			}
			else
			{
				fadeEvent == FadeEvent::FADED_IN;
			}
		}
	}
};

int main() {
	// Initialization
	int screenWidth = 1280;
	int screenHeight = 720;

	raylib::Color textColor(BLACK);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	raylib::Window w(screenWidth, screenHeight, "Geometry Dash");
	raylib::AudioDevice audioDevice;

	raylib::Texture2D playerTextureCube("assets/player/cube.png");

	CurrentScreen currentScreen = CurrentScreen::TITLE;

	SetTargetFPS(FRAMERATE);
	SetWindowFullScreen(&w, screenWidth, screenHeight, 0.1);

	Player player(LoadTexture("assets/player/cube.png"), raylib::Vector2{25.0, 0.0});

	raylib::Camera2D playerCamera;
	playerCamera.target = player.position;
	float winSizeZoomMultiplier = 1.0;

	// We use this camera to render zoom when changing the size of the window.
	// This camera is used when the playerCamera is not in use.
	// Otherwise, this is handled by playerCamera.
	raylib::Camera2D winSizeCamera(VECTOR2_ZERO, VECTOR2_ZERO);

	Sprite logo(LoadTexture("assets/gui/logo.png"), raylib::Vector2{0.0f, 150.0f}, 1.0f);
	Sprite background(LoadTexture("assets/level/background1-3.png"), raylib::Vector2{0.0f, -500.0f}, 0.75f, 0.0f, raylib::Color{0x46a0ffff});
	Sprite ground(LoadTexture("assets/level/ground-long.png"), raylib::Vector2{0.0f, 900.0f}, 0.5f, 0.0f, raylib::Color{0x4a44ffff});
	Sprite groundShadow(LoadTexture("assets/level/groundSquareShadow_001.png"), ground.position, 0.5f, 0.0f, raylib::Color{0xffffff88});
	Sprite groundLine(LoadTexture("assets/level/floorLine_001.png"), raylib::Vector2{0.0f, ground.position.y}, 0.75f, 0.0f, raylib::Color{0xffffffff});
	SpriteButton garageButton(LoadTexture("assets/gui/iconSelectorButton.png"));
	SpriteButton playButton(LoadTexture("assets/gui/levelSelectorButton.png"));
	SpriteButton createButton(LoadTexture("assets/gui/levelCreateButton.png"));

	FadeScreen fader(BLACK);
	raylib::Music menuLoop("assets/sounds/soundEffects/menuLoop.mp3");

	// Main game loop
	while (!w.ShouldClose()) // Detect window close button or ESC key
	{
		// Time elapsed since the last frame
		float deltaTime = GetFrameTime();
		menuLoop.Update();
		background.position.x -= 250 * deltaTime;
		background.position.x = fmodf(background.position.x, background.texture.width * background.scale);
		ground.position.x -= 750 * deltaTime;
		ground.position.x = fmodf(ground.position.x, ground.texture.width * ground.scale);

		if (IsKeyPressed(KEY_F11)) {
			winSizeZoomMultiplier = SetWindowFullScreen(&w, screenWidth, screenHeight, deltaTime);
		}

		garageButton.RefreshButtonScale(deltaTime, winSizeCamera);
		playButton.RefreshButtonScale(deltaTime, winSizeCamera);
		createButton.RefreshButtonScale(deltaTime, winSizeCamera);
		fader.RefreshFade(deltaTime);

		if (playButton.hasBeenReleased && !fader.hasStartedFadingIn) {
			fader.FadeIn(0.25f);
		}

		// TODO: Add a camera system.
		playerCamera.zoom = winSizeZoomMultiplier;
		winSizeCamera.zoom = winSizeZoomMultiplier;

		switch (currentScreen) {
			case CurrentScreen::TITLE:
				if (!menuLoop.IsPlaying())
				{
					menuLoop.Play();
				}
				logo.position.x = logo.MoveToWinPercentage(raylib::Vector2{0.5f}).x;
				groundLine.position.x = groundLine.MoveToWinPercentage(raylib::Vector2{0.5f}).x;
				garageButton.position = garageButton.MoveToWinPercentage(raylib::Vector2{0.35f, 0.5f});
				playButton.position = playButton.MoveToWinPercentage(raylib::Vector2{0.5f, 0.5f});
				createButton.position = createButton.MoveToWinPercentage(raylib::Vector2{0.65f, 0.5f});
				break;
			case CurrentScreen::IN_LEVEL:
				player.UpdatePlayer(deltaTime);
				break;
		}

		// Draw
		BeginDrawing();
			switch (currentScreen)
			{
				case CurrentScreen::TITLE:
					BeginMode2D(winSizeCamera);
						ClearBackground(WHITE);
						background.Draw();
						background.Draw(raylib::Vector2{background.position.x + background.texture.width * background.scale});
						background.Draw(raylib::Vector2{background.position.x + background.texture.width * 2 * background.scale});
						ground.Draw();
						ground.Draw(raylib::Vector2{ground.position.x + ground.texture.width * ground.scale});
						groundShadow.Draw();
						groundShadow.Draw(raylib::Vector2{static_cast<float>(GetMonitorWidth(GetCurrentMonitor())), ground.position.y+groundShadow.texture.height*groundShadow.scale}, 180.0f);
						BeginBlendMode(BLEND_ADDITIVE);
							groundLine.Draw();
						EndBlendMode();
						logo.Draw();
						garageButton.Draw();
						playButton.Draw();
						createButton.Draw();
					EndMode2D();
					break;
				case CurrentScreen::IN_LEVEL:
					BeginMode2D(playerCamera);
						ClearBackground(WHITE);
						player.Draw();
						textColor.DrawText("Congrats! You created your first window!", 190, 200, 20);
					EndMode2D();
					break;
				default:
					ClearBackground(BLACK);
					break;
			}
		fader.Draw();
		EndDrawing();
	}

	return 0;
}
