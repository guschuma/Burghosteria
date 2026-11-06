#include "screens/gameplay.hpp"
#include "global.hpp"
#include "button.hpp"
#include <iostream>
#include "structs/queue.hpp"

#include "screens/store.hpp"

#include "raymath.h"
#include <cstring>

#include "structs/fx_queue.hpp"

#define ORDER_X_POS 222
#define ORDER_Y_POS 40
#define ORDER_LINE_HEIGHT 9
#define RECIPE_WIDTH 160 // Tamanho visual da caixa ("papel") da ordem


Burger_Stack burger;
Client_Queue ghosts;

AVL_Hash avl;

Texture2D recipe_book;
Texture2D background;
Texture2D table;
Texture2D lamp;
Texture2D red_spawn_fx;
	

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
Button spell((Rectangle){121, 46, 28, 18}, "", BOOK_COLOR, SELECTED_BOOK_COLOR, CLICKED_BOOK_COLOR, -14);

Spell_Matrix spells_matrix(spell, (Vector2){2, 2}, (Vector2){121, 46}, 6, 3);

int id_of_previous_button_held;
bool ingred_is_being_summoned = false;

bool in_middle_of_spawning_ingred = false;
float timer_till_ingred_spawn = 0.0f;
int ingred_to_be_added;

int number_non_perish[3] = {0,0,0}; 
void reset_numbers_food();

int current_order_id = -1;
char current_order[MAX_LENGTH_RECIPE];
float char_timer_recipe = -0.75f; // Time ate adicionar o proximo ingrediente na animacao rapida de nova receita
int old_number_ingred; // usado para destaque de ingrediente
float time_on_same_ingred = 0.0f;

int n_food_on_stock[12] = {0};
Food_Heap meat_heap;
Food_Heap eggs_heap;
Food_Heap baco_heap;

double second_passed = 0.0f; // Sempre entre zero e um

double time_till_shop = 0.0f;



int player_money_cents = INITIAL_PLAYER_MONEY;
int player_lifes;

bool should_exit_gameplay = false;
double leaving_anim_timer = 0.0f;


Vector2 cursor_position; // Congelado quando no shop

FX_Queue fx_queue;


void draw_ui();
void update_spell_button_logic();
void increase_food_stock_based_on_time(double *time, int button_id);
bool requires_prep(int ingred_id);
void root_perish_food();
void update_perish_food_values();
void draw_order();
void update_order();


bool can_be_placed_in_burger(int id_of_button_released);
bool prep_stock_is_empty(int id_of_button_held);

void draw_cool_text_with_outline(const char *text, float x_pos, float y_pos, Color tint, int font_size = 13);



void init_gameplay(){
	player_money_cents = 1000;
	player_lifes = 3;
	should_exit_gameplay = false;
	leaving_anim_timer = 0.0f; // TODO: Adicionar animacao de entrada
	
	reset_numbers_food();
	for(int i = 15; i <= 17; i++){
		spells_matrix.change_height(i, 35);
	}
	//spells_matrix.reset_all_text();
	player_money_cents = INITIAL_PLAYER_MONEY;
	time_till_shop = 0.0f;
	char_timer_recipe = -0.75f;
	current_order_id = -1;
	time_on_same_ingred = 0.0f;
	in_middle_of_spawning_ingred = false;
	timer_till_ingred_spawn = 0.0f;
	

	ghosts.clear();

	ghosts.set_number_clients_round(10);
	update_order();
}

void init_sprites_gameplay(){
	recipe_book = LoadTexture("assets/sprites/recipe_book.png");
	table = LoadTexture("assets/sprites/table.png");
	background = LoadTexture("assets/sprites/background.png");
	lamp = LoadTexture("assets/sprites/lamp.png");
	burger.init_ingred_sprites();
	red_spawn_fx = LoadTexture("assets/sprites/fx/red_spawn.png");
	init_ghost_sprites();
	fx_queue.load();
	init_sprites_shop();
}

void end_gameplay(){
	burger.clear();
	ghosts.clear();
	fx_queue.clear();
	meat_heap.terminate();
	eggs_heap.terminate();
	baco_heap.terminate();
	end_ghost_sprites();
	end_sprites_shop();
}

void end_sprites_gameplay(){
	UnloadTexture(recipe_book);
	UnloadTexture(table);
	UnloadTexture(background);
	UnloadTexture(lamp);
	UnloadTexture(red_spawn_fx);
	burger.unload_sprites();
	fx_queue.unload();
}


void update_gameplay(){
	is_camera_shaking = false;
	if(in_shop){
		update_shop();
		// Dont update the usual gameplay
		return;
	}
	if(should_exit_gameplay){
		leaving_anim_timer += GetFrameTime();
		if(leaving_anim_timer >= 1) {
			current_screen = Screen::START_MENU;
			leaving_anim_timer = 1;
		}

		return;
	}

	// Update itens pereciveis
	second_passed += GetFrameTime();
	time_till_shop += GetFrameTime();
	char_timer_recipe += GetFrameTime();
	if(in_middle_of_spawning_ingred) {
		is_camera_shaking = true;
		timer_till_ingred_spawn += GetFrameTime();
		if(timer_till_ingred_spawn > 0.15) {
			timer_till_ingred_spawn = 0.0f;
			in_middle_of_spawning_ingred = false;

			burger.insert_ingredient(number_to_ingred(ingred_to_be_added));
		}
	}
	burger.update();
	if(time_till_shop >= SHOP_TIME) {
		time_till_shop = 0;
		in_shop = true;
		is_leaving_shop = false;
		init_shop();
	}
	if(second_passed >= 1) {
		second_passed = 0;
		root_perish_food();
		ghosts.decrease_all_patience();
	}
	Emotion first_ghost_emotion;
	if(ghosts.update(second_passed, first_ghost_emotion) == QUEUE_MOVED){
		update_order();
		if(first_ghost_emotion == ANGRY) {
			player_lifes--;
			std::cout << "Player lost life! Current: " << player_lifes << "\n\n";
			should_exit_gameplay = (player_lifes == 0)? true : false;
			
		}

	}
	spells_matrix.update();
	fx_queue.update();
	cursor_position = cursor_pos();

	if(IsKeyPressed(KEY_BACKSPACE)){
		fx_queue.insert(remove_fx, {(float)burger_middle_x, burger_bottom_y - burger.get_first_height() - 30});
		burger.remove_ingredient();
		burger.print();
		
	}
	if(IsKeyPressed(KEY_ENTER)){
		Burger_Stack burg_order;
		for(char ingred_char : current_order){
			burg_order.insert_ingredient(char_to_ingred(ingred_char));
		}
		bool is_correct = burger.is_identical(burg_order);

		if(is_correct) {
			ghosts.make_first_leave(HAPPY);
			player_money_cents += burger.get_price();
		}
		else ghosts.make_first_leave(ANGRY);

		burger.start_burger_leaving_anim(0.0f);
		//burger.clear();
		burg_order.clear();
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
	if(should_exit_gameplay) draw_circle_anim(1 - leaving_anim_timer);
	else draw_circle_anim(1);

	BeginTextureMode(pixelated_screen);
		ClearBackground(RAYWHITE);
		DrawTexture(background, 0, 0, WHITE);
		ghosts.draw();
		DrawTexture(table, 0, 0, WHITE);
		int lamp_anim_frame = (int)(second_passed * 4.0) % 2; // Zero ou um
		DrawTextureRec(lamp, {60.0f * lamp_anim_frame, 0, 60, 100}, {0, 40}, WHITE);

		burger.draw();

		DrawTexture(recipe_book, 0, 0, WHITE); // Desenha sprite do livro aberto

		draw_order();

		spells_matrix.draw();
		if(time_prep_spell_held > 0) draw_ingred_being_summoned(id_of_previous_button_held, time_prep_spell_held);
		fx_queue.draw();
		draw_ui();
		if(in_shop){
			draw_shop();
		}
		
		draw_cursor();
		
	EndTextureMode();

	
	draw_pixelated_screen(pixelated_screen, should_exit_gameplay);
	
	
}




bool requires_prep(int ingred_id){
	//return ingred_id >= Burger_Stack::Ingredient::q && ingred_id <= Burger_Stack::Ingredient::b;
	return (ingred_id >= -3 && ingred_id <= -1) || (ingred_id >= 12 && ingred_id <= 14);
	// Tomate nao eh preparavel no meu jogo (para ter simetria nos botoes)
}

void draw_ingred_being_summoned(int ingred_id, float animation_time){ // ingred_id eh recebido como id do butao (-3 a 14)
	
	if(ingred_id < 0) ingred_id += 3;
	if(ingred_id > 11) ingred_id -= 3;
	int ingred_height = cursor_position.y - spawning_height * 0.5;
	//Color color;
	//int ingred_vis_h;
	//ingred_properties((Burger_Stack::Ingredient)ingred_id, &color, &ingred_vis_h);

	float sqrt_tim = sqrt(sqrt(animation_time));
	//color = Fade(color, 4*(- animation_time*animation_time + animation_time));
	float x_offset = 36 * cos(6 * PI * sqrt_tim) * (1 - sqrt_tim);
	float y_offset = 24 * sin(6 * PI * sqrt_tim) * (1 - sqrt_tim);
	draw_ingredient_at(ingred_id, cursor_position.x - ingred_width * 0.5 + x_offset, ingred_height + y_offset, 
									4*(- sqrt_tim*sqrt_tim + sqrt_tim));

	DrawTextureRec(red_spawn_fx, {64.0f * (int)(animation_time * 8), 0, 64, 64}, 
						{cursor_position.x - 32, ingred_height - 15.0f}, 
						WHITE);
	//DrawRectangleRounded((Rectangle){(float)(cursor_position.x - ingred_width * 0.5) + x_offset,
//									ingred_height + y_offset,
//									(float)ingred_width, 
//									(float)ingred_vis_h,}, 
//									0.5, 12, color);
}

void draw_ui(){
	std::string player_money = "G$ " +
							 std::to_string(player_money_cents / 100) + "." + 
							 +   (player_money_cents % 100 < 10 ? "0" : "") +
							 std::to_string(player_money_cents % 100);
	draw_cool_text_with_outline(player_money.c_str(), 6, 2, P_CREAM);

	int ghost_number = ghosts.get_number_of_invisible_ghosts();
	if(ghost_number != 0) {
		std::string queue_count = std::to_string(ghost_number);
		queue_count = "+" + queue_count;
		draw_cool_text_with_outline(queue_count.c_str(), 103, 4, P_CREAM); // Numero em queue
	}
	

	// Desenha proximos elementos do heap de comida pereciveis
	int text_size = 8;
	for(int i = 0; i < 4; i++){
		int meat_heap_value = meat_heap.get_id_value(i);
		if(meat_heap_value != EMPTY_HEAP) 
			draw_cool_text_with_outline((std::to_string(meat_heap_value) + "s").c_str(), 121, 147 + text_size*(i + 1), P_CREAM);

		int eggs_heap_value = eggs_heap.get_id_value(i);
		if(eggs_heap_value != EMPTY_HEAP) 
			draw_cool_text_with_outline((std::to_string(eggs_heap_value) + "s").c_str(), 151, 147 + text_size*(i + 1), P_CREAM);

		int baco_heap_value = baco_heap.get_id_value(i);
		if(baco_heap_value != EMPTY_HEAP) 
			draw_cool_text_with_outline((std::to_string(baco_heap_value) + "s").c_str(), 181, 147 + text_size*(i + 1), P_CREAM);
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
	meat_heap.terminate();
	eggs_heap.terminate();
	baco_heap.terminate();

	meat_heap.initialize();
	eggs_heap.initialize();
	baco_heap.initialize();

	for(int initial_time = 40; initial_time < 240; initial_time += 40){
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


	if(id_of_button_released != ERROR_CODE && !requires_prep(id_of_button_released) && can_be_placed_in_burger(id_of_button_released) && !burger.is_in_anim()) { // Um ingrediente nao perecivel foi apertado
	// + Com comida no estoque caso seja finito e burguer nao esta em animacao de saida
		//spells_matrix.update_percentage_held(time_prep_spell_held, id_of_button_held);
		if(id_of_button_released < 3 || id_of_button_released > 8) { // The stock is not infinite
			n_food_on_stock[id_of_button_released]--;
			spells_matrix.set_text_value_at_ingred(n_food_on_stock[id_of_button_released], id_of_button_released);
		}
		fx_queue.insert(FX::summoning, {(float)burger_middle_x, (float)burger_bottom_y - burger.get_height() - spawning_height - 7});
		
		
		//burger.insert_ingredient(number_to_ingred(id_of_button_released));
		ingred_to_be_added = id_of_button_released;
		in_middle_of_spawning_ingred = true;
		timer_till_ingred_spawn = 0.0f;
		
		burger.print();
	}
	if(id_of_button_held != ERROR_CODE) id_of_previous_button_held = id_of_button_held;
}

void increase_food_stock_based_on_time(double *time, int button_id){
	if(*time == 1.0f){
		// Botao foi pressionado por tempo suficiente. Inserir comida no estoque correspondente
		if(button_id < 0 && button_id >= -3) {
			number_non_perish[button_id + 3]--;
			if(number_non_perish[button_id + 3] <= -1) number_non_perish[button_id + 3] = 0; // Nao pode ser negativo
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

		// Coloca efeito 
		fx_queue.insert(red_spawn, {cursor_position.x, cursor_position.y - 16});
	}
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

// Draw_order tambem faz update de ingrendiente highlight
void draw_order(){  // Char time recipe = timer de animacao, comeca em -0.75 
	int number_ingreds = burger.get_number_ingred();

	int up_to_char = (int)(char_timer_recipe * 4);

	int order_x_pos = ORDER_X_POS;
	float x_v = char_timer_recipe;
	
	if(char_timer_recipe < 1) {
		float anim_curve = abs((x_v + 0.75) * (x_v - 0.5) * (x_v - 1)); // "Parabola" para animacao da receita chegando 
		order_x_pos += 200 * anim_curve; 
	}
	
	Rectangle recipe_visual_paper = (Rectangle){(float)order_x_pos - 3, ORDER_Y_POS - 4, RECIPE_WIDTH, INTERNAL_HEIGHT - ORDER_Y_POS + 10};
	DrawRectangleRec(recipe_visual_paper, P_CREAM); // desenha papel (quadrado branco atras)
	DrawRectangleLinesEx(recipe_visual_paper, 1, P_TAN);
	if(current_order_id == -1) return;

	// Desenha selecao de receita
	if(number_ingreds < strlen(current_order)){
		
		DrawRectangle(recipe_visual_paper.x, 
						ORDER_Y_POS + (2 * MAX_LENGTH_RECIPE - number_ingreds * 2 - 3) * ORDER_LINE_HEIGHT + 2, 
						recipe_visual_paper.width,
						2 * ORDER_LINE_HEIGHT + 1, P_MAUVE);

		int button_pos = ingred_to_button_map(char_to_ingred(current_order[number_ingreds]));
		int button_lin = button_pos / 3;
		int button_col = button_pos % 3;

		
		if(old_number_ingred == number_ingreds && char_timer_recipe > 1){
			time_on_same_ingred += GetFrameTime();
		}
		else time_on_same_ingred = 0.0f;

		float fade = time_on_same_ingred > 1.5f? cos(1 * PI * time_on_same_ingred) * cos(1 * PI * time_on_same_ingred) : 0;
		DrawRectangleLines(121 + 30 * button_col, 46 + 20 * button_lin, 28, 18, {255, 255, 255, (unsigned char)(255.0 * fade)});
		old_number_ingred = number_ingreds;
	}

	draw_cool_text(("Pedido: " + std::to_string(current_order_id)).c_str(), order_x_pos, ORDER_Y_POS, P_RED);
	
	int cur_caracter = 0;
	for(int i = 0; current_order[i] != '\0' && i < up_to_char; i++){
		Burger_Stack::Ingredient current_ingred = char_to_ingred(current_order[i]);
		int order_y_pos =  ORDER_Y_POS + (2 * MAX_LENGTH_RECIPE - cur_caracter++ - 2) * ORDER_LINE_HEIGHT;
		draw_cool_text(ingred_to_name(current_ingred, false).c_str(), order_x_pos, order_y_pos, P_DARK); 
		order_y_pos -= ORDER_LINE_HEIGHT;
		cur_caracter++;
		draw_cool_text(ingred_to_name(current_ingred, true).c_str(), order_x_pos, order_y_pos, P_DARK); 

		DrawLine(order_x_pos, order_y_pos + 2, order_x_pos + RECIPE_WIDTH, order_y_pos + 2, P_MAUVE); // Desenha separacoes de linha entre igrendien
		
		//draw_cool_text((std::to_string(char_to_ingred(current_order[i]))).c_str(), ORDER_X_POS, ORDER_Y_POS + (MAX_LENGTH_RECIPE - cur_caracter) * ORDER_LINE_HEIGHT, BLACK); // TODO: mudar para mostarr nomes dos ingredientes

	}
	

}

void update_order(){
	std::cout << "\n\nUpdated order!" << "\n";
	current_order_id = ghosts.get_first_order_id();
	char_timer_recipe = -0.75f; // Demora um pouco para comecar a tocar a animacao da ordem
	if(current_order_id == -1) return;

	strcpy(current_order, avl.search(current_order_id));
	std::cout << "Got order:" << current_order << "\n\n";
	
}

void draw_cool_text_with_outline(const char *text, float x_pos, float y_pos, Color tint, int font_size){
	draw_cool_text(text, x_pos + 1, y_pos, P_WINE, font_size);
	draw_cool_text(text, x_pos + 1, y_pos + 1, P_WINE, font_size);
	draw_cool_text(text, x_pos, y_pos, tint, font_size);
}