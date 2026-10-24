#include "screens/store.hpp"
#include "global.hpp"
#include "screens/gameplay.hpp"
#include <iostream> // Pra debug
#include "raymath.h"
#include <cstdlib> // Para random
double shop_entering_anim_time = 0.0f; // Entre 0 e 1;

double time_on_shop = 0.0f;
bool in_shop = false;
bool is_leaving_shop = false;

#define MAX_PRICE_VARIATION_PERC 0.3 // para cima ou para baixo

Vector2 buttons_origin;
Vector2 buttons_leave_dest = {-245, -83};
Vector2 buttons_come_dest = {10, 60};
Vector2 buttons_velocity = {0, 0};
Button shop_button((Rectangle){10, 60, 80, 40}, "teste", WHITE, GRAY, GRAY, -39);

Button_Matrix stock_buys(shop_button, (Vector2){2, 2}, buttons_origin, 2, 3);
Vector2 position_deb = {0, 0};

int stock_prices_cents[6] = {0,0,0,0,0,0};
int n_items_bought_this_time[6] = {0,0,0,0,0,0};
int expiration_stock[3] = {0,0,0};
void update_stock_at_id(int price_cents, int id_stock, int time_to_expire = -1);
int generate_random_price(int base_price, int id_stock = -1);
void shop_button_interaction();
void init_shop(){
	player_money_cents = 1000;
	buttons_origin = buttons_leave_dest;
	stock_buys.set_origin(buttons_origin);
	buttons_velocity = {0, 0};
	update_stock_at_id(200, 0);
	update_stock_at_id(300, 1);
	update_stock_at_id(400, 2);

	update_stock_at_id(500, 3, 40);
	update_stock_at_id(300, 4, 40);
	update_stock_at_id(1100, 5, 40);

	for(int &n_bought : n_items_bought_this_time){ n_bought = 0;}

	for(int i = 0; i < 6; i++) {
		stock_buys.update_blocked(i, stock_prices_cents[i] > player_money_cents);
	}
}
void update_shop(){
	if(!is_leaving_shop) time_on_shop += GetFrameTime();
	Vector2 destination = is_leaving_shop? buttons_leave_dest : buttons_come_dest;
	destination.y += 2 * cos(0.5 * PI * time_on_shop); // Flutuante
	buttons_velocity = Vector2Subtract(destination, buttons_origin);
	buttons_velocity = Vector2Scale(buttons_velocity, 3* GetFrameTime());
	buttons_origin = Vector2Add(buttons_origin, buttons_velocity);
	stock_buys.set_origin(buttons_origin);
	if(shop_entering_anim_time < 1.0f && !is_leaving_shop){
		shop_entering_anim_time += 2 * GetFrameTime();
		shop_entering_anim_time = (shop_entering_anim_time > 1.0f)? 1.0f : shop_entering_anim_time;
	}
	if(shop_entering_anim_time > 0.0f && is_leaving_shop){
		shop_entering_anim_time -= GetFrameTime();
		if(shop_entering_anim_time < 0.0f){
			shop_entering_anim_time = 0.0f;
			in_shop = false; // Saiu completamente da loja
		}
	}
	if(IsKeyDown(KEY_UP)){
		position_deb.y -= 0.5;
	}
	if(IsKeyDown(KEY_DOWN)){
		position_deb.y += 0.5;
	}
	if(IsKeyDown(KEY_LEFT)){
		position_deb.x -= 0.5;
	}
	if(IsKeyDown(KEY_RIGHT)){
		position_deb.x += 0.5;
	}
	
	if(time_on_shop >= IN_SHOP_TIME){
		time_on_shop = 0.0f;
		is_leaving_shop = true;
	}
	
	shop_button_interaction();
	stock_buys.update();
}

void draw_shop(){
	
	DrawRectangle(0, 0, INTERNAL_WIDTH, INTERNAL_HEIGHT, (Color){20, 20, 140, (unsigned char)(shop_entering_anim_time * 100.0f)});
	DrawRectangle(position_deb.x, position_deb.y, 1, 1, RED);
	//std::cout << position_deb.x << " : " << position_deb.y << "\n";
	stock_buys.draw();
	// Shop time ui tempo acaba
	DrawRectangleGradientV(0, INTERNAL_HEIGHT * (IN_SHOP_TIME - time_on_shop) / IN_SHOP_TIME, 4, INTERNAL_HEIGHT, BLUE, PURPLE);
}

// Time to expire default eh -1
void update_stock_at_id(int price_cents, int id_stock, int time_to_expire){
	price_cents = generate_random_price(price_cents, id_stock);
	time_to_expire = generate_random_price(time_to_expire);
	std::string price_text = "G$ " +
							 std::to_string(price_cents / 100) + "." + 
							 +   (price_cents % 100 < 10 ? "0" : "") +
							 std::to_string(price_cents % 100);

	std::string button_text;
	switch(id_stock){
		case 0: button_text = "+1 cheddar"; break;	// Espaço usado para centralizar texto. Bem feio mas funciona
		case 1: button_text = "+1 cenoura"; break;
		case 2: button_text = "+1 suico"; break;
		case 3: button_text = "+1 carne"; break;
		case 4: button_text = "+1 ovo"; break; // Espaço usado para centralizar texto. Bem feio mas funciona
		case 5: button_text = "+1 bacon"; break;
		default: 
		return;
	}
	if(time_to_expire != -1) {
		expiration_stock[id_stock - 3] = time_to_expire;
		button_text += "  |  " + std::to_string(time_to_expire) + "s";
	}
	button_text = button_text+ "\n" + price_text;
	stock_prices_cents[id_stock] = price_cents;
	
	stock_buys.set_text_at_id(button_text, id_stock);

	
}

int generate_random_price(int base_price, int id_stock){
	if(id_stock != -1) { base_price += 100 * n_items_bought_this_time[id_stock];}
	float max_variation = base_price * MAX_PRICE_VARIATION_PERC;

	float rand_percentage = (rand() % 101) * 0.01;
	int variation = 2 * max_variation * rand_percentage - max_variation;
	
	return base_price + variation;
}

void shop_button_interaction(){
	int id_of_button_released = stock_buys.id_of_button_released();
	if(id_of_button_released == ERROR_CODE) return;

	int price_stock_interacted = stock_prices_cents[id_of_button_released];
	if(player_money_cents < price_stock_interacted) return;
	
	switch(id_of_button_released){
		case 0: update_stock_at_id(200, 0);  // TODO: Organizar melhor isso aqui
		number_non_perish[0]++;
		break;
		case 1:	update_stock_at_id(300, 1); 
		number_non_perish[1]++;
		break;
		case 2:	update_stock_at_id(400, 2); 
		number_non_perish[2]++;
		break;
		

		case 3: meat_heap.insert(expiration_stock[0]);
		update_stock_at_id(500, 3, 40); 
		
		break; 
		case 4: eggs_heap.insert(expiration_stock[1]);
		update_stock_at_id(300, 4, 40); 
		
		break;
		case 5: baco_heap.insert(expiration_stock[2]);
		update_stock_at_id(1100, 5, 40); 
		break;
		default: return;
	}

	player_money_cents -= price_stock_interacted;
	n_items_bought_this_time[id_of_button_released]++;

	for(int i = 0; i < 6; i++) {
		stock_buys.update_blocked(i, stock_prices_cents[i] > player_money_cents);
	}

	spell_matrix_update();
}