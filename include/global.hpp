#pragma once // Inclua esse header apenas uma vez
#include "raylib.h"
// Actual resolution 300 x 180
constexpr int WIDTH_SCREEN = 1200;
constexpr int HEIGHT_SCREEN = 720;

const int INTERNAL_WIDTH = 300;
const int INTERNAL_HEIGHT = 180;

constexpr int FPS = 60;

extern bool is_camera_shaking; // Talvez nao usado

#define ERROR_CODE (int)(-4)
#define EMPTY_HEAP (int)(-10)

enum class Screen {
    GAMEPLAY,
	START_MENU,
};

// Definicao de cores
#define P_CREAM       (Color){255, 244, 224, 255}
#define P_LIGHT_BLUE  (Color){143, 204, 203, 255}
#define P_TEAL        (Color){ 68, 148, 137, 255}
#define P_DARK_TEAL   (Color){ 40,  87,  99, 255}
#define P_PURPLE_DARK (Color){ 47,  43,  92, 255}
#define P_PURPLE      (Color){ 75,  59, 156, 255}
#define P_BLUE        (Color){ 69, 124, 214, 255}
#define P_YELLOW      (Color){242, 182,  61, 255}
#define P_ORANGE      (Color){212, 110,  51, 255}
#define P_RED         (Color){227,  66,  98, 255}
#define P_RED_DARK    (Color){148,  53,  61, 255}
#define P_WINE        (Color){ 87,  37,  59, 255}
#define P_MAUVE       (Color){156, 101, 108, 255}
#define P_TAN         (Color){209, 180, 140, 255}
#define P_OLIVE       (Color){180, 186,  71, 255}
#define P_GREEN       (Color){109, 140,  50, 255}
#define P_DARK        (Color){ 44,  27,  46, 255}
#define P_PEACH       (Color){251, 177, 139, 255}
#define P_SALMON      (Color){216, 138,  87, 255}

#define TRANSPARENT   (Color){  0,   0,   0,   0}


extern RenderTexture2D pixelated_screen;
extern Screen current_screen;

extern Font pixelify_sans;

void initStart();

void draw_pixelated_screen(RenderTexture2D render_screen);
void draw_cursor();
void draw_cool_text(const char *text, float x_pos, float y_pos, Color tint, int font_size = 13);
Vector2 cursor_pos(); // Em tamanho interno (320 x 180)