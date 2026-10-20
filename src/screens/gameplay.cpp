#include "screens/gameplay.hpp"
#include "global.hpp"
#include "button.hpp"
#include <iostream>

#include "screens/store.hpp"

#include "raymath.h"
Vector2 position = {0, 0}; // Para debug
Burger_Stack burger;

/*case Burger_Stack::Ingredient::z: return "Pao"; 
		case Burger_Stack::Ingredient::k: return "Ketchup";
		case Burger_Stack::Ingredient::m: return "Mostarda"; 
		case Burger_Stack::Ingredient::p: return "Picles";
		case Burger_Stack::Ingredient::a: return "Alface"; 
		case Burger_Stack::Ingredient::s: return "Queijo Suico"; <- trocado
		case Burger_Stack::Ingredient::q: return "Queijo Cheddar";
		case Burger_Stack::Ingredient::c: return "Cebola";
		case Burger_Stack::Ingredient::t: return "Tomate";   <- Trocados
		case Burger_Stack::Ingredient::n: return "Carne"; 
		case Burger_Stack::Ingredient::o: return "Ovo Frito"; 
		case Burger_Stack::Ingredient::b: return "Bacon";
		*/


double time_prep_spell_held = 0.0f;
Button spell((Rectangle){121, 46, 28, 18}, "", BOOK_COLOR, SELECTED_BOOK_COLOR, CLICKED_BOOK_COLOR, -13);

Spell_Matrix spells_matrix(spell, (Vector2){2, 2}, (Vector2){121, 46}, 6, 3);

int id_of_previous_button_held;
bool ingred_is_being_summoned = false;

int number_non_perish[3] = {0,0,0}; 
void reset_numbers_food();

int n_food_on_stock[12] = {0};
Food_Heap meat_heap;
Food_Heap eggs_heap;
Food_Heap baco_heap;

double mili_seconds_passed = 0.0f; // Sempre entre zero e um

double time_till_shop = 0.0f;



int player_money_cents = INITIAL_PLAYER_MONEY;

Vector2 cursor_position; // Congelado quando no shop

void draw_ui();
void update_spell_button_logic();
void increase_food_stock_based_on_time(double *time, int button_id);
bool requires_prep(int ingred_id);
void root_perish_food();
void update_perish_food_values();

bool can_be_placed_in_burger(int id_of_button_released);
bool prep_stock_is_empty(int id_of_button_held);
void update_gameplay(){
	if(in_shop){
		update_shop();
		// Dont update the usual gameplay
		return;
	}
	// Update itens pereciveis
	mili_seconds_passed += GetFrameTime();
	time_till_shop += GetFrameTime();
	if(time_till_shop >= SHOP_TIME) {
		time_till_shop = 0;
		in_shop = true;
		is_leaving_shop = false;
		init_shop();
	}
	if(mili_seconds_passed >= 1) {
		mili_seconds_passed = 0;
		root_perish_food();
	}

	spells_matrix.update();
	burger.update_falling_ingred();
	cursor_position = cursor_pos();

	// Debug square
	if(IsKeyDown(KEY_UP)){
		position.y -= 0.5;
	}
	if(IsKeyDown(KEY_DOWN)){
		position.y += 0.5;
	}
	if(IsKeyDown(KEY_LEFT)){
		position.x -= 0.5;
	}
	if(IsKeyDown(KEY_RIGHT)){
		position.x += 0.5;
	}
	if(IsKeyPressed(KEY_BACKSPACE)){
		burger.remove_ingredient();
		burger.print();
	}
	
	update_spell_button_logic();

	if(IsKeyPressed(KEY_SPACE)){
		in_shop = true;
		is_leaving_shop = false;
		init_shop();
	}
}


void root_perish_food(){
	meat_heap.decrease_by_one();
	eggs_heap.decrease_by_one();
	baco_heap.decrease_by_one();

	update_perish_food_values();
}

void draw_gameplay(){
	BeginTextureMode(pixelated_screen);
		ClearBackground(RAYWHITE);
		DrawRectangle(0, 37, 300, 180, LIGHTGRAY);
		//DrawRectangle(position.x, position.y, 1, 1, RED);
		//std::cout << "x: " << position.x << "\ty:" << position.y << "\n";
		draw_burger();

		spells_matrix.draw();
		if(time_prep_spell_held > 0) draw_ingred_being_summoned(id_of_previous_button_held, time_prep_spell_held);
		draw_ui();
		if(in_shop){
			draw_shop();
		}
		draw_cursor();
		
	EndTextureMode();

	
	draw_pixelated_screen(pixelated_screen);
	
}



void draw_burger(){
	int burger_num_ingred = burger.get_number_ingred();

	int cur_ingred_height;
	Burger_Stack::Ingredient cur_ingred;

	for(int pos = 0; pos < burger_num_ingred; pos++){
		burger.get_ingred_start_bottom(pos, &cur_ingred, &cur_ingred_height);

		draw_ingredient(cur_ingred_height, cur_ingred);

		if(cur_ingred_height > burger_bottom_y) { // Fora da tela
			break;
		}
	}
}

void draw_ingredient(int height, Burger_Stack::Ingredient ingred){
	Color color;
	int ingred_vis_h;
	ingred_properties(ingred, &color, &ingred_vis_h);
	DrawRectangleRounded((Rectangle){(float)(burger_middle_x - ingred_width * 0.5),
									(float)(burger_bottom_y - (ingred_vis_h + height)),
									(float)ingred_width, 
									(float)ingred_vis_h,}, 
									0.5, 12, color);

}

void ingred_properties(int ingred_id, Color *color, int *ingred_vis_h){
	switch(ingred_id){
		case -3:
		case Burger_Stack::Ingredient::q: *color = WHITE;	break;
		case -2:
		case Burger_Stack::Ingredient::c: *color = PURPLE;	break;
		case -1:
		case Burger_Stack::Ingredient::s: *color = YELLOW;	break;
		case Burger_Stack::Ingredient::z: *color = ORANGE; 	break;
		case Burger_Stack::Ingredient::k: *color = RED;		break;
		case Burger_Stack::Ingredient::m: *color = YELLOW;	break;
		case Burger_Stack::Ingredient::p: *color = GREEN;	break;
		case Burger_Stack::Ingredient::a: *color = LIME;	break;
		case Burger_Stack::Ingredient::t: *color = RED;		break;
		case 12:
		case Burger_Stack::Ingredient::n: *color = PINK;	break; 
		case 13:
		case Burger_Stack::Ingredient::o: *color = WHITE;	break;
		case 14:
		case Burger_Stack::Ingredient::b: *color = RED;		break;
		default: *color = BLACK;
	}
	if(ingred_id >= -3 && ingred_id <= -1) {
		*ingred_vis_h = ingred_vis_heig[ingred_id + 3];
		return;
	}
	if(ingred_id >= 12 && ingred_id <= 14) {
		*ingred_vis_h = ingred_vis_heig[ingred_id - 3];
		return;
	}
	if(ingred_id == ERROR_CODE) {
		*ingred_vis_h = 1;  // Invalid
		return;
	}
	*ingred_vis_h = ingred_vis_heig[ingred_id];
}


bool requires_prep(int ingred_id){
	//return ingred_id >= Burger_Stack::Ingredient::q && ingred_id <= Burger_Stack::Ingredient::b;
	return (ingred_id >= -3 && ingred_id <= -1) || (ingred_id >= 12 && ingred_id <= 14);
	// Tomate nao eh preparavel no meu jogo (para ter simetria nos botoes)
}

void draw_ingred_being_summoned(int ingred_id, float animation_time){
	int ingred_height = cursor_position.y - spawning_height * 0.5;
	Color color;
	int ingred_vis_h;
	ingred_properties((Burger_Stack::Ingredient)ingred_id, &color, &ingred_vis_h);

	animation_time = sqrt(sqrt(animation_time));
	color = Fade(color, 4*(- animation_time*animation_time + animation_time));
	float x_offset = 36 * cos(6 * PI * animation_time) * (1 - animation_time);
	float y_offset = 24 * sin(6 * PI * animation_time) * (1 - animation_time);
	DrawRectangleRounded((Rectangle){(float)(cursor_position.x - ingred_width * 0.5) + x_offset,
									ingred_height + y_offset,
									(float)ingred_width, 
									(float)ingred_vis_h,}, 
									0.5, 12, color);
}

void draw_ui(){
	std::string player_money = "G$ " +
							 std::to_string(player_money_cents / 100) + "." + 
							 +   (player_money_cents % 100 < 10 ? "0" : "") +
							 std::to_string(player_money_cents % 100);
	draw_cool_text(player_money.c_str(), 6, 2, BLACK);

	// Desenha proximos elementos do heap de comida pereciveis
	int text_size = 8;
	for(int i = 0; i < 4; i++){
		int meat_heap_value = meat_heap.get_id_value(i);
		if(meat_heap_value != EMPTY_HEAP) draw_cool_text((std::to_string(meat_heap_value) + "s").c_str(), 122, 147 + text_size*(i + 1), BLACK);

		int eggs_heap_value = eggs_heap.get_id_value(i);
		if(eggs_heap_value != EMPTY_HEAP) draw_cool_text((std::to_string(eggs_heap_value) + "s").c_str(), 152, 147 + text_size*(i + 1), BLACK);

		int baco_heap_value = baco_heap.get_id_value(i);
		if(baco_heap_value != EMPTY_HEAP) draw_cool_text((std::to_string(baco_heap_value) + "s").c_str(), 182, 147 + text_size*(i + 1), BLACK);
	}

	// Shop time ui

	DrawRectangleGradientV(0, INTERNAL_HEIGHT * (SHOP_TIME - time_till_shop) / SHOP_TIME, 4, INTERNAL_HEIGHT, GREEN, BLUE);

}

void reset_numbers_food(){ // Includes reseting spells_matrix
	for (int ingred_non_perish = 0; ingred_non_perish < 3; ingred_non_perish++)
	{
		number_non_perish[ingred_non_perish] = START_NUM_NON_PERISH_NOT_PREP;
		spells_matrix.set_text_value_at_ingred(START_NUM_NON_PERISH_NOT_PREP, ingred_non_perish - 3);
	}
	for(int ingred_id = 0; ingred_id < 3; ingred_id++){
		n_food_on_stock[ingred_id] = START_NUM_FOOD_STOCK;
		spells_matrix.set_text_value_at_ingred(START_NUM_FOOD_STOCK, ingred_id);
	}
	for(int ingred_id = 9; ingred_id < 12; ingred_id++){
		n_food_on_stock[ingred_id] = START_NUM_FOOD_STOCK;
		spells_matrix.set_text_value_at_ingred(START_NUM_FOOD_STOCK, ingred_id);
	}
	
	meat_heap.initialize();
	eggs_heap.initialize();
	baco_heap.initialize();

	for(int initial_time = 20; initial_time < 600; initial_time += 20){
		meat_heap.insert(initial_time);
		eggs_heap.insert(initial_time);
		baco_heap.insert(initial_time);

	}
	spells_matrix.set_text_value_at_ingred(meat_heap.get_current_size(), 12, meat_heap.get_root_value());
	spells_matrix.set_text_value_at_ingred(eggs_heap.get_current_size(), 13, eggs_heap.get_root_value());
	spells_matrix.set_text_value_at_ingred(baco_heap.get_current_size(), 14, baco_heap.get_root_value());
}

void update_spell_button_logic(){
	int id_of_button_held = spells_matrix.id_of_button_held();
	int id_of_button_released = spells_matrix.id_of_button_released();
	if(requires_prep(id_of_button_held) && !prep_stock_is_empty(id_of_button_held)) {
		if(id_of_previous_button_held != id_of_button_held) time_prep_spell_held = 0.0f;
		time_prep_spell_held += GetFrameTime();
		time_prep_spell_held = Clamp(time_prep_spell_held, 0.0, 1.0);
		ingred_is_being_summoned = true;
		spells_matrix.update_percentage_held(time_prep_spell_held, id_of_button_held);

		increase_food_stock_based_on_time(&time_prep_spell_held, id_of_button_held);
		
	}
	else {
		ingred_is_being_summoned = false;
		if(time_prep_spell_held > 0 && requires_prep(id_of_previous_button_held)){
			time_prep_spell_held -= GetFrameTime();
			spells_matrix.update_percentage_held(time_prep_spell_held, id_of_previous_button_held);
		}
		else if(time_prep_spell_held > 0){
			time_prep_spell_held = 0;
		}
	}


	if(id_of_button_released != ERROR_CODE && !requires_prep(id_of_button_released) && can_be_placed_in_burger(id_of_button_released)) { // Um ingrediente nao perecivel foi apertado
	// + Com comida no estoque caso seja finito
		//spells_matrix.update_percentage_held(time_prep_spell_held, id_of_button_held);
		if(id_of_button_released < 3 || id_of_button_released > 8) { // The stock is not infinite
			n_food_on_stock[id_of_button_released]--;
			spells_matrix.set_text_value_at_ingred(n_food_on_stock[id_of_button_released], id_of_button_released);
		}
		burger.insert_ingredient(number_to_ingred(id_of_button_released));
		burger.print();
	}
	if(id_of_button_held != ERROR_CODE) id_of_previous_button_held = id_of_button_held;
}

void increase_food_stock_based_on_time(double *time, int button_id){
	if(*time == 1.0f){
		// Botao foi pressionado por tempo suficiente. Inserir comida no estoque correspondente
		if(button_id < 0 && button_id >= -3) {
			number_non_perish[button_id + 3]--;
			if(number_non_perish[button_id + 3] < -1) number_non_perish[button_id + 3] = 0; // Nao pode ser negativo
			else {
				n_food_on_stock[button_id + 3]++;
				spells_matrix.set_text_value_at_ingred(n_food_on_stock[button_id + 3], button_id + 3); // Update food stock (aumentou 1)
				spells_matrix.set_text_value_at_ingred(number_non_perish[button_id + 3], button_id); // Update comida nao preparada (diminui 1)
			}
		}
		if(button_id >= 12 && button_id <= 14){
			if(button_id == 12 && !meat_heap.is_empty()) {
				meat_heap.remove_id(0);
				n_food_on_stock[button_id - 3]++;
				spells_matrix.set_text_value_at_ingred(meat_heap.get_current_size(), 12, meat_heap.get_root_value());
			}
			if(button_id == 13 && !eggs_heap.is_empty()) {
				eggs_heap.remove_id(0);
				n_food_on_stock[button_id - 3]++;
				spells_matrix.set_text_value_at_ingred(eggs_heap.get_current_size(), 13, eggs_heap.get_root_value());
			}
			if(button_id == 14 && !baco_heap.is_empty()) {
				baco_heap.remove_id(0);
				n_food_on_stock[button_id - 3]++;
				spells_matrix.set_text_value_at_ingred(baco_heap.get_current_size(), 14, baco_heap.get_root_value());
			}
			spells_matrix.set_text_value_at_ingred(n_food_on_stock[button_id - 3], button_id - 3);
		}
		
		burger.print();
		*time = 0.0f;
	}
}

void start_gameplay(){
	reset_numbers_food();
	for(int i = 15; i <= 17; i++){
		spells_matrix.change_height(i, 35);
	}
	//spells_matrix.reset_all_text();
	player_money_cents = INITIAL_PLAYER_MONEY;
	time_till_shop = 0.0f;
}

void end_gameplay(){
	burger.clear();
	meat_heap.terminate();
	eggs_heap.terminate();
	baco_heap.terminate();
}

// Comida pode ser colocada no burguer se for um ingrediente de estoque infinito OU eh estoque finito com quantidade nao nula
bool can_be_placed_in_burger(int id_of_button_released){
	return n_food_on_stock[id_of_button_released] > 0 || (id_of_button_released >=3 && id_of_button_released <= 8);
}

bool prep_stock_is_empty(int id_of_button_held){
	if(id_of_button_held < 0 && id_of_button_held >= -3) return number_non_perish[id_of_button_held + 3] == 0;
	
	if(id_of_button_held == 12) return meat_heap.is_empty();
	if(id_of_button_held == 13) return eggs_heap.is_empty();
	if(id_of_button_held == 14) return baco_heap.is_empty();

	return false;
}

void spell_matrix_update() {
	spells_matrix.update();
	for(int i = 0; i < 3; i++){
		spells_matrix.set_text_value_at_ingred(n_food_on_stock[i], i);
	}
	update_perish_food_values(); 
	for (int ingred_non_perish = 0; ingred_non_perish < 3; ingred_non_perish++)
	{
		spells_matrix.set_text_value_at_ingred(number_non_perish[ingred_non_perish], ingred_non_perish - 3);
	}
}

void update_perish_food_values(){
	spells_matrix.set_text_value_at_ingred(meat_heap.get_current_size(), 12, meat_heap.get_root_value());
	spells_matrix.set_text_value_at_ingred(eggs_heap.get_current_size(), 13, eggs_heap.get_root_value());
	spells_matrix.set_text_value_at_ingred(baco_heap.get_current_size(), 14, baco_heap.get_root_value());
}