#include "raylib.h"
#include "raymath.h"

#include "screens/gameplay.hpp"
#include "global.hpp"

#include "structs/history.hpp"
#include "structs/avl_hash.hpp"
#include "structs/queue.hpp"
#include <string>
#include <iostream>

void end_everything();

int main(int argc, char *argv[]) { 
	if(argc < 3){
		fprintf(stderr, " Uso: %s PEDIDOS=<pedidos> HIST=<historico>\n" , argv[0]) ;
		return -1;
	}
	//std::string hist_name(argv[2]);
	//hist_name = "bin/"+hist_name;

	std::string recipes_name(argv[1]);
	recipes_name = "bin/" + recipes_name;

	avl.recipes_file_name = recipes_name;
	avl.read_from_file();
	//avl.print();

	//History hist;

	//hist.history_file_name = hist_name;
	//hist.read_from_file();
	//hist.sort();
	
	//hist.print();
	//hist.insert("teste", 21, true);
	//hist.print();


	InitWindow(WIDTH_SCREEN, HEIGHT_SCREEN, "Burghosteria!");
	SetTargetFPS(FPS);
	initStart();
	HideCursor();
	srand(time(nullptr));

	start_gameplay();

	while (!WindowShouldClose()) {
		// Event Handling + Update
		switch (current_screen) {
			case Screen::GAMEPLAY:
				update_gameplay();
				break;
			case Screen::START_MENU:
				break;
		}
		// Drawing 
		switch (current_screen) {
			case Screen::GAMEPLAY:
				draw_gameplay();
				
				break;
			case Screen::START_MENU:
				break;
		}
	}

	end_everything();
	CloseWindow();

	return 0;
}

void end_everything(){
	end_gameplay();
}