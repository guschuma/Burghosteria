#include "global.hpp"
#include "screens/defeat.hpp"
#include "structs/history.hpp"
#include "raylib.h"
#include <cstring>


char username[MAX_NAME_SIZE];
float blinking_cursor_time = 0.0f;
bool is_cursor_shown = true; // Cursor da posicao de caractere
int letter_count = 0;

void update_defeat(History &name_history){
	blinking_cursor_time += GetFrameTime();
	if(blinking_cursor_time > 0.5){
		blinking_cursor_time = 0.0f;
		is_cursor_shown = !is_cursor_shown;
	}
	bool should_leave_defeat_screen = get_name();

	if(should_leave_defeat_screen){
		name_history.insert(username, current_score, true);
		current_screen = Screen::START_MENU;
	}
}

void draw_defeat(){
	BeginTextureMode(pixelated_screen);
        ClearBackground(P_DARK);

		char username_shown[MAX_NAME_SIZE + 1];
		strcpy(username_shown, username);
		if(letter_count != MAX_NAME_SIZE - 1) {
			username_shown[letter_count] = is_cursor_shown? '_' : ' ';
			username_shown[letter_count + 1] = '\0';
		}
		std::string score_text = "Pontuação: " + std::to_string(current_score);
		draw_cool_text(score_text.c_str(), INTERNAL_WIDTH * 0.4, INTERNAL_HEIGHT * 0.2, P_TAN);

		draw_cool_text("Digite o seu nome: ", INTERNAL_WIDTH * 0.4, INTERNAL_HEIGHT * 0.3, P_TAN);
		draw_cool_text(username_shown, INTERNAL_WIDTH * 0.4, INTERNAL_HEIGHT * 0.4, P_CREAM);
        draw_cursor();

    EndTextureMode();

	draw_pixelated_screen(pixelated_screen, false);
}

void init_defeat(){
	blinking_cursor_time = 0.0f;
}

bool get_name(){
	int key = GetCharPressed();
	while(key > 0){
		if ((key >= 32) && (key <= 125) && (letter_count < MAX_NAME_SIZE - 1))
		{
			username[letter_count] = (char)key;
			username[letter_count+1] = '\0'; 
			letter_count++;
		}

		key = GetCharPressed(); 
	}

	if (IsKeyPressed(KEY_BACKSPACE))
	{
		letter_count--;
		if (letter_count < 0) letter_count = 0;
		username[letter_count] = '\0';
	}

	return IsKeyPressed(KEY_ENTER);
}