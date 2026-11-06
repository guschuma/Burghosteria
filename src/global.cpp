#include "global.hpp"
#include "raymath.h"

RenderTexture2D pixelated_screen;

RenderTexture2D circle_anim;
Screen current_screen;

Font pixelify_sans;
bool is_camera_shaking = false; // Defined in global.cpp, atualizado em gameplay.cpp
bool should_update_camera_shake = false; // Divide frequencia em dois
Vector2 camera_shake = Vector2Zero();
bool should_leave_game = false;
const char *characters =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789"
	" !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
	"àáâãçéêíóôõú";


void initStart(){
	pixelated_screen = LoadRenderTexture(INTERNAL_WIDTH, INTERNAL_HEIGHT);
	circle_anim = LoadRenderTexture(INTERNAL_WIDTH, INTERNAL_HEIGHT);
	current_screen = Screen::START_MENU;
	SetTextureFilter(pixelated_screen.texture, TEXTURE_FILTER_POINT); // for crisp pixelart *chef kiss* :)
 
	const int FONT_SIZE = 99;


	unsigned int file_size = 0;

	unsigned char *file_data =
    LoadFileData(
        "assets/font/PixelifySans-Regular.ttf",
        &file_size
    );

	pixelify_sans.baseSize = FONT_SIZE;
	int glyph_count;
	int *codepoints = LoadCodepoints(characters, &glyph_count);
	pixelify_sans.glyphCount = glyph_count;
	pixelify_sans.glyphs = LoadFontData(
	file_data,
		file_size,
		FONT_SIZE,
		codepoints,
		glyph_count,
		FONT_BITMAP
	);

	UnloadCodepoints(codepoints);

	Image atlas = GenImageFontAtlas(
		pixelify_sans.glyphs,
		&pixelify_sans.recs,
		glyph_count,
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


void draw_pixelated_screen(RenderTexture2D render_screen, bool is_leaving_or_entering){
	BeginDrawing();
	ClearBackground(BLACK);
	should_update_camera_shake = !should_update_camera_shake;
	if(is_camera_shaking && should_update_camera_shake) {
		camera_shake.x = rand() % 3 - 1;
		camera_shake.y = rand() % 3 - 1;
	}
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
	DrawRectangleLinesEx({0, 0, WIDTH_SCREEN, HEIGHT_SCREEN}, 4, P_DARK);
	if(is_leaving_or_entering){
		DrawTexturePro(
		circle_anim.texture,
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
	}
	
    EndDrawing();
}

Vector2 cursor_pos(){
	return Vector2Scale(GetMousePosition(), 0.25f);
}

void draw_cursor(){
	Vector2 c_pos = cursor_pos();
	c_pos.x = (int)c_pos.x;
	c_pos.y = (int)c_pos.y;
	DrawRectangle(c_pos.x - 1, c_pos.y, 3, 1, P_DARK_TEAL); // TODO: bem feio, talvez melhorar depois?
	DrawRectangle(c_pos.x, c_pos.y - 1, 1, 3, P_DARK_TEAL);
	DrawRectangleV(c_pos, (Vector2){1, 1}, P_TEAL);
}

void draw_cool_text(const char *text, float x_pos, float y_pos, Color tint, int font_size){ // Ira converter x e y para coordenadas inteiras
	DrawTextEx(pixelify_sans, text, (Vector2){((float)(int)x_pos), (float)(int)y_pos}, font_size, 0, tint);
}

void draw_circle_anim(float radius_size, Vector2 center_pos){ // Raio do circulo vai de 0 a 1
	if(radius_size == 1){ // Nao desenhe animacao
		BeginTextureMode(circle_anim);
		EndTextureMode();
		return;
	}
	// Circle transition
	radius_size *= radius_size * INTERNAL_WIDTH; // Makes it more smooth and slower (quadratic)
	BeginTextureMode(circle_anim);
	ClearBackground(P_DARK);
	
	BeginBlendMode(BLEND_SUBTRACT_COLORS);
	DrawCircleV(center_pos, radius_size, WHITE);
	EndBlendMode();
	EndTextureMode();
}