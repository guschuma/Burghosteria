#include "global.hpp"
#include "raymath.h"

RenderTexture2D pixelated_screen;
Screen current_screen;

Font pixelify_sans;


void initStart(){
	pixelated_screen = LoadRenderTexture(INTERNAL_WIDTH, INTERNAL_HEIGHT);
	current_screen = Screen::GAMEPLAY;
	SetTextureFilter(pixelated_screen.texture, TEXTURE_FILTER_POINT); // for crisp pixelart *chef kiss* :)
 
	const int FONT_SIZE = 99;
	const int GLYPH_COUNT = 95;

	unsigned int file_size = 0;

	unsigned char *file_data =
    LoadFileData(
        "assets/font/PixelifySans-Regular.ttf",
        &file_size
    );

	pixelify_sans.baseSize = FONT_SIZE;
	pixelify_sans.glyphCount = GLYPH_COUNT;

	pixelify_sans.glyphs = LoadFontData(
		file_data,
		file_size,
		FONT_SIZE,
		nullptr,
		GLYPH_COUNT,
		FONT_BITMAP
	);

	Image atlas = GenImageFontAtlas(
		pixelify_sans.glyphs,
		&pixelify_sans.recs,
		GLYPH_COUNT,
		FONT_SIZE,
		1,
		0
	);

	pixelify_sans.texture = LoadTextureFromImage(atlas);

	SetTextureFilter(
		pixelify_sans.texture,
		TEXTURE_FILTER_POINT
	);

	UnloadImage(atlas);
	UnloadFileData(file_data);
}


void draw_pixelated_screen(RenderTexture2D render_screen){
	BeginDrawing();
	ClearBackground(BLACK);
	DrawTexturePro(
		render_screen.texture,
		{ 0, 0,
			(float)render_screen.texture.width,
			-(float)render_screen.texture.height, },
		{ 0, 0,
			(float)WIDTH_SCREEN,
			(float)HEIGHT_SCREEN, },
		{ 0, 0 },
		0.0f,
		WHITE
	);
    EndDrawing();
}

Vector2 cursor_pos(){
	return Vector2Scale(GetMousePosition(), 0.25f);
}

void draw_cursor(){
	DrawRectangleV(cursor_pos(), (Vector2){1, 1}, BLACK);
}

void draw_cool_text(const char *text, float x_pos, float y_pos, Color tint, int font_size){ // Ira converter x e y para coordenadas inteiras
	DrawTextEx(pixelify_sans, text, (Vector2){((float)(int)x_pos), (float)(int)y_pos}, font_size, 0, tint);
}