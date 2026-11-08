#include "raylib.h"
#include "raymath.h"

#include "screens/gameplay.hpp"
#include "screens/menu.hpp"
#include "screens/defeat.hpp"
#include "screens/score.hpp"
#include "global.hpp"
#include "sound.hpp"

#include "structs/history.hpp"
#include "structs/avl_hash.hpp"
#include "structs/queue.hpp"
#include <string>
#include <iostream>

void end_everything();

int main(int argc, char *argv[]) { 
	if(argc < 3){
		fprintf(stderr, "Comando invalido\nUso: %s PEDIDOS=<pedidos> HIST=<historico>\n" , argv[0]) ;
		return -1;
	}
	std::string hist_name(argv[2]);
	hist_name = "bin/" + hist_name;

	std::string recipes_name(argv[1]);
	recipes_name = "bin/" + recipes_name;

	avl.recipes_file_name = recipes_name;
	avl.read_from_file();
	//avl.print();

	History hist;

	hist.history_file_name = hist_name;
	

	SetConfigFlags(FLAG_MSAA_4X_HINT); // Colocar isso para evitar ruídos MUITO ALTOs e infrequentes
	SetAudioStreamBufferSizeDefault(4096);          // e isso 
	InitAudioDevice();



	InitWindow(WIDTH_SCREEN, HEIGHT_SCREEN, "Burghosteria!");
	SetTargetFPS(FPS);
	initStart();
	HideCursor();
	srand(time(nullptr));

	init_sprites_gameplay();
	init_menu();
	init_gameplay();
	init_score(hist);

	load_music_and_sound();

	while (!WindowShouldClose() && !should_leave_game) {
		Screen old_screen = current_screen;
		// Event Handling + Update
		switch (current_screen) {
			case Screen::GAMEPLAY:
				update_gameplay();
				break;
			case Screen::START_MENU:
				update_menu();
				break;
			case Screen::DEFEAT:
				update_defeat(hist);
				break;
			case Screen::SCORE:
				update_score(hist);
				break;
		}
		// Drawing 
		switch (current_screen) {
			case Screen::GAMEPLAY:
				draw_gameplay();
				break;
			case Screen::START_MENU:
				draw_menu();
				break;
			case Screen::DEFEAT:
				draw_defeat();
				break;
			case Screen::SCORE:
				draw_score(hist);
				break;
		}
		total_menu_time += GetFrameTime(); 

		// Music
		play_music_based_on_screen(old_screen);

	}

	end_everything();
	CloseWindow();

	return 0;
}

void end_everything(){
	end_gameplay();
	fx_queue.clear();
	end_sprites_gameplay();
	UnloadRenderTexture(pixelated_screen);
	UnloadFont(pixelify_sans);
	UnloadTexture(background);
	unload_music_and_sound();
	CloseAudioDevice(); 
	end_menu();
}