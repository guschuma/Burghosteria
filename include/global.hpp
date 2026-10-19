#pragma once // Inclua esse header apenas uma vez
#include "raylib.h"
// Actual resolution 300 x 180
constexpr int WIDTH_SCREEN = 1200;
constexpr int HEIGHT_SCREEN = 720;

const int INTERNAL_WIDTH = 300;
const int INTERNAL_HEIGHT = 180;

constexpr int FPS = 60;

#define ERROR_CODE (int)(-4)
#define EMPTY_HEAP (int)(-10)

enum class Screen {
    GAMEPLAY,
	START_MENU,
};

extern RenderTexture2D pixelated_screen;
extern Screen current_screen;

extern Font pixelify_sans;

void initStart();

void draw_pixelated_screen(RenderTexture2D render_screen);
void draw_cursor();
void draw_cool_text(const char *text, float x_pos, float y_pos, Color tint, int font_size = 13);
Vector2 cursor_pos(); // Em tamanho interno (320 x 180)