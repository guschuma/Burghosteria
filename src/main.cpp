#include "raylib.h"
#include "raymath.h"

#include "screens/gameplay.hpp"
#include "global.hpp"
#include <cstdlib> // Para random
#include <iostream>

void end_everything();

int main() { 
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