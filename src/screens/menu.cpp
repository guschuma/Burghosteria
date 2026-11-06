#include "screens/menu.hpp"
#include "button.hpp"
#include "global.hpp"
#include "screens/gameplay.hpp"
#include "raylib.h"

Button menu_button((Rectangle){10, 60, 76, 15}, "teste", P_PURPLE_DARK, P_PURPLE, P_DARK, -36);

Button_Matrix menu_buttons(menu_button, (Vector2){2, 2}, (Vector2){(INTERNAL_WIDTH - 60) * 0.5, 70}, 4, 1);

void init_menu(){
    menu_buttons.set_text_at_id("Começar",    0);
    menu_buttons.set_text_at_id("Pontuação",  1);
    menu_buttons.set_text_at_id("Como jogar", 2);
    menu_buttons.set_text_at_id("Sair",       3);
}

void update_menu(){
    menu_buttons.update();
    if(menu_buttons.id_of_button_released() == 0) { // Botao de comecar a jogar
        current_screen = Screen::GAMEPLAY;
        init_gameplay();
    }
    if(menu_buttons.id_of_button_released() == 3){
        should_leave_game = true;
    }
}

void draw_menu(){
    BeginTextureMode(pixelated_screen);
        ClearBackground(P_DARK);
        menu_buttons.draw();
        draw_cursor();

    EndTextureMode();

	draw_pixelated_screen(pixelated_screen, false);
}