#include "screens/menu.hpp"
#include "button.hpp"
#include "global.hpp"
#include "screens/gameplay.hpp"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "sound.hpp"

#define BURGER_SUMMON_ANIM_TIME_START 1.5f
Button menu_button((Rectangle){10, 60, 76, 15}, "teste", P_PURPLE_DARK, P_PURPLE, P_DARK, -36);

Button_Matrix menu_buttons(menu_button, (Vector2){2, 2}, (Vector2){150, 100}, 4, 1);
Texture2D menu_witch;
Texture2D menu_burger;

Vector2 menu_witch_pos = {-180, 200};
Vector2 menu_witch_dest = {0, 0};
bool just_entered_menu = true;
float time_till_burger_summoned = 0.0f;
bool burger_summoned = false;
bool leaving_menu = false;
float leaving_menu_timer = 0.0f;
Screen next_screen;

void init_menu(){
    menu_buttons.set_text_at_id("\t\t\t\t\tComeçar",    0);
    menu_buttons.set_text_at_id("\t\tPontuação",  1);
    menu_buttons.set_text_at_id("\t\tComo jogar", 2);
    menu_buttons.set_text_at_id("\t\t\t\t\t\t\t\t\t\t\tSair",       3);

    menu_witch = LoadTexture("assets/sprites/menu_witch.png");
    menu_burger = LoadTexture("assets/sprites/menu_burger.png");
}

void update_menu(){
    menu_buttons.update(); 
    
    switch(menu_buttons.id_of_button_released()){
        case 0:
            next_screen = Screen::GAMEPLAY;
            fx_queue.clear();
            leaving_menu = true;
            play_sound(trans_down_s);
            break;
        case 1:
            current_screen = Screen::SCORE;
            fx_queue.clear();
            just_entered_menu = true;
            break;
        
        case 3:
            should_leave_game = true;
            total_menu_time = 0.0f;
            break;
            
        default: break;
    }

    if(leaving_menu){
        leaving_menu_timer += GetFrameTime();
        if(leaving_menu_timer > 1.0f){
            current_screen = next_screen;
            init_gameplay();
            just_entered_menu = true; // Para ser true na proxima vez que entra nessa tela
            leaving_menu = false;
            leaving_menu_timer = 0.0f;
        }
    }

    background_dest.y = 10 * sin(total_menu_time) + INTERNAL_HEIGHT * 0.3;
	update_background_pos();
	background_pos.x = total_menu_time;
	background_pos.x *= 10;
	background_pos.x = (int)background_pos.x % INTERNAL_WIDTH;

    menu_witch_dest = {0, 3.0f * cos(total_menu_time * 2.0f)};

    Vector2 witch_velocity = Vector2Subtract(menu_witch_dest, menu_witch_pos);
    witch_velocity = Vector2Scale(witch_velocity, 3 * GetFrameTime());
    menu_witch_pos = Vector2Add(menu_witch_pos, witch_velocity);

    if(!burger_summoned && time_till_burger_summoned < BURGER_SUMMON_ANIM_TIME_START) {
        time_till_burger_summoned += GetFrameTime();
        float tbs = time_till_burger_summoned;
        menu_buttons.set_origin({180.0f + 200.0f * (tbs - 1.5f) * (tbs * tbs - 0.75f), 100});
    }
    else{
        if(!burger_summoned){
            burger_summoned = true;
            fx_queue.insert(remove_fx, {104, 134});
            fx_queue.insert(summoning, {104, 134});
            play_sound(summoning_s);
        }
    }

    fx_queue.update();
}

void draw_menu(){
     if(just_entered_menu) {
        menu_witch_pos = {-600, 600};
        just_entered_menu = false;
        time_till_burger_summoned = 0.0f;
        burger_summoned = false;
        menu_buttons.set_origin({300, 90});
        leaving_menu = false;
        leaving_menu_timer = 0.0f;
        fx_queue.clear();
    }

    if(leaving_menu){
        draw_circle_anim(1 - leaving_menu_timer);
    }

    BeginTextureMode(pixelated_screen);
        ClearBackground(P_DARK);
        draw_background();
        menu_buttons.draw();
        

        bool witch_frame = ((int)(4 * total_menu_time) % 16) == 4;
        DrawTextureRec(menu_witch, {180.0f * (int)witch_frame, 0, 180, 200}, menu_witch_pos, WHITE);


        Vector2 menu_burger_pos = {5.0f * cos(1.5f * total_menu_time), 5.0f * sin(2.0f * total_menu_time)};
        if(burger_summoned) DrawTexture(menu_burger, menu_burger_pos.x, menu_burger_pos.y, WHITE);
        fx_queue.draw();
        draw_cursor();
    EndTextureMode();


	draw_pixelated_screen(pixelated_screen, leaving_menu);   
}

void end_menu(){
    UnloadTexture(menu_burger);
    UnloadTexture(menu_witch);
}