// Define tela de pontuacao
#include "screens/score.hpp"
#include "raylib.h"
#include "global.hpp"
#include <iostream> // TODO: remover 
#include "button.hpp"

#define NUMBER_NAMES_PER_PAGE 16
int current_page = 0;

Button score_button((Rectangle){10, 60, 75, 15}, "", P_PURPLE_DARK, P_PURPLE, P_DARK, -36);

Button_Matrix score_buttons(score_button, (Vector2){2, 2}, (Vector2){INTERNAL_WIDTH * 0.5 - 76, 114}, 3, 2);
bool in_alphabetical_order = false;
int page = 0;

void init_score(History &hist){
	hist.read_from_file();
	hist.sort();

	score_buttons.set_text_at_id("\t\t\t\t\t\t\t\t\t\t\t   <-   ", 0); // \t para centralizar texto. Bem feio, mas funciona
	score_buttons.set_text_at_id("\t\t\t\t\t\t\t\t\t\t\t\t  -> ",   1); 
    score_buttons.set_text_at_id("\t\t\t\t\t\t\t\t\t\t <---   ", 2);
	score_buttons.set_text_at_id("\t\t\t\t\t\t\t\t\t\t  --->   ", 3);

	score_buttons.set_text_at_id("   Ordenação:\n      Numérica", 4);
    score_buttons.set_text_at_id("\t\t\t\t\t\t\t\t\t\tSair", 5);
	score_buttons.change_height(4, 30);
}

void update_score(History &hist){
	score_buttons.update();

	switch(score_buttons.id_of_button_released()){
	case 0: current_page--; break;
	case 1: current_page++; break;
	case 2: current_page -= 100; break;
	case 3: current_page += 100; break;
	case 4: 
		in_alphabetical_order = !in_alphabetical_order;

		if(in_alphabetical_order) score_buttons.set_text_at_id("   Ordenação:\n   Alfabética", 4);
		else                      score_buttons.set_text_at_id("   Ordenação:\n    Numérica", 4);

		hist.sort(in_alphabetical_order);
		break;
	case 5: current_screen = Screen::START_MENU; break;
	default: break;
	}

	if(current_page < 0) current_page = 0;

	background_dest.y = 10 * sin(total_menu_time) + INTERNAL_HEIGHT * 0.65;
	update_background_pos();
	background_pos.x = total_menu_time;
	background_pos.x *= 10;
	background_pos.x = (int)background_pos.x % INTERNAL_WIDTH;
}

void draw_score(History &hist){
	 BeginTextureMode(pixelated_screen);
        ClearBackground(P_DARK);
		draw_background();

        int current_size = hist.get_current_size();
		if(current_page > current_size  / NUMBER_NAMES_PER_PAGE) current_page = current_size  / NUMBER_NAMES_PER_PAGE;

		for(int i = 0; i < NUMBER_NAMES_PER_PAGE; i++){
			char *name = hist.get_name_at(i +  NUMBER_NAMES_PER_PAGE * current_page);
			if(name == nullptr) break;
			int score = hist.get_score_at(i + NUMBER_NAMES_PER_PAGE * current_page);

			int x_offset = (int)(i % 2) * INTERNAL_WIDTH * 0.5;

			draw_cool_text(name, 70 + x_offset, (i / 2) * 14, P_CREAM);
			std::string score_text = std::to_string(score);
			
			draw_cool_text_with_outline(score_text.c_str(), 0.15 * INTERNAL_WIDTH + x_offset, 14 * (i / 2), P_ORANGE);

			if(!in_alphabetical_order)
				 draw_cool_text_with_outline((std::to_string(i + 1 + NUMBER_NAMES_PER_PAGE * current_page) + "]").c_str(), 4 + x_offset, 14 * (i / 2), P_PEACH);
		}
		
		std::string page_text = "Pág " + std::to_string(current_page) + " / " + std::to_string(current_size / NUMBER_NAMES_PER_PAGE);
		draw_cool_text_with_outline(page_text.c_str(), 154, 166, P_CREAM);
		DrawLine(INTERNAL_WIDTH * 0.5, 4, INTERNAL_WIDTH * 0.5, 110, P_PURPLE_DARK);
		score_buttons.draw();
        draw_cursor();

    EndTextureMode();
	//std::cout << "x: " << cursor_pos().x << "y:" << cursor_pos().y << "\n";
	draw_pixelated_screen(pixelated_screen, false);
}