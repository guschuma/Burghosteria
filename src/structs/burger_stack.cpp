#include "structs/burger_stack.hpp"
#include <iostream>
#include "log_timer.hpp"
#include "global.hpp"

#define INGRED_VIS_H 25
#define INGRED_VIS_W 60
//    _______________                     __
//   |               |					    |
//   |===============|  -.					|-> ingred_vis_heig
//   |###############|   |-> ..._tallness   |
//    -------X-------   _'  			  _/					<---- height (relative to bottom of burger)
//           |
//           v
//     burger_middle_x
//   (. . . . . . . .)
//           |
//           v
//     ingred_width

const int ingred_tallness[12] = {10,10,8,15,10,8,12,10,11,10,9,10};
const int ingred_vis_heig[12] = {13,10,14,17,13,17,14,13,15,15,12,11}; // TODO: Deletar isso aqui

      int burger_middle_x = 48;
const int burger_bottom_y = 160;
const int ingred_width = 60;
const int spawning_height = 70;
const float ingred_falling_accel = 4.0f;

Texture2D ingred_sprites;
Texture2D bottom_bread;

void Burger_Stack::insert_ingredient(Burger_Stack::Ingredient ingred){
	if(ingred == Ingredient::OFF_LIMITS || in_anim) return;
	Node* new_node = new Node;
	new_node->ingred = ingred;
	new_node->next = this->top;
	new_node->desired_height = this->height;
	new_node->height = this->height + spawning_height;
	new_node->falling = true;

	this->top = new_node;
	this->price  += ingredient_price(ingred);
	this->height += ingred_tallness[(int)ingred];
	number_ingred++;
}

void Burger_Stack::remove_ingredient(){
	Node* previous_top = this->top;
	if(previous_top == nullptr){return;} // Lista vazia
	this->top = previous_top->next;
	this->price -= Burger_Stack::ingredient_price(previous_top->ingred);
	this->height -= ingred_tallness[(int)previous_top->ingred];
	number_ingred--;
	delete previous_top;
}

std::string ingred_to_string(Burger_Stack::Ingredient ing){
	switch(ing){
		case Burger_Stack::Ingredient::q: return "Queijo Cheddar";
		case Burger_Stack::Ingredient::c: return "Cebola";
		case Burger_Stack::Ingredient::s: return "Queijo Suico";

		case Burger_Stack::Ingredient::z: return "Pao"; 
		case Burger_Stack::Ingredient::k: return "Ketchup";
		case Burger_Stack::Ingredient::m: return "Mostarda"; 
		case Burger_Stack::Ingredient::p: return "Picles";
		case Burger_Stack::Ingredient::a: return "Alface"; 
		case Burger_Stack::Ingredient::t: return "Tomate";
		
		case Burger_Stack::Ingredient::n: return "Carne"; 
		case Burger_Stack::Ingredient::o: return "Ovo Frito"; 
		case Burger_Stack::Ingredient::b: return "Bacon";
		default: return "Ingred. desconhecido";
	}
}

void Burger_Stack::print() const{
	Node* aux = this->top;
	std::cout << "\n";
	std::cout << "Preco:  " << this->price << "\n";
	std::cout << "N:      " << this->number_ingred << "\n";
	std::cout << "Altura: " << this->height << "\n\n";
	while(aux != nullptr){
		std::cout << ingred_to_string(aux->ingred) << " \t- Alt: " << aux->height
		<< "\t Des: " << aux->desired_height << "\t Vel.: " << aux->velocity << "\n";
		aux = aux->next;
	}
	std::cout << "\n";
}

void Burger_Stack::clear(){
	if(this->top == nullptr) {return;} // Ja esta vazio
	Node* aux = this->top;
	while(aux != nullptr){
		Node* next = aux->next;
		delete aux;
		aux = next;
	}
	this->top = nullptr;
	height = 0;
	price = 0;
	number_ingred = 0;
}

bool Burger_Stack::is_identical(const Burger_Stack &other_stack){
	Node* aux_first = this->top;
	Node* aux_second = other_stack.top;

	while(aux_first != nullptr && aux_second != nullptr){
		if(aux_first->ingred != aux_second->ingred) {return false;}
		aux_first = aux_first->next;
		aux_second = aux_second->next;
	}
	return aux_first == nullptr && aux_second == nullptr;
}

// Pos eh indexado no zero
void Burger_Stack::get_ingred_start_bottom(int pos, Burger_Stack::Ingredient *ingred, int *height) const{
	if(pos >= this->number_ingred) {
		*ingred = Burger_Stack::Ingredient::OFF_LIMITS;
		*height = -1;
	}

	Node* aux = this->top;
	for(int i = 0; i < this->number_ingred - pos - 1; i++){
		aux = aux->next;
	}
	*ingred = aux->ingred;
	*height = aux->height;
}

void Burger_Stack::update_falling_ingred(){
	Node* aux = this->top;

	while(aux != nullptr && aux->falling){
		aux->height += aux->velocity;
		aux->velocity -= ingred_falling_accel * GetFrameTime();

		if(aux->height <= aux->desired_height){
			aux->height = aux->desired_height;
			aux->falling = false;
			aux->velocity = 0.0f;
			break;
		}

		aux = aux->next;
	}
}

void Burger_Stack::start_burger_leaving_anim(float initial_time){
	in_anim = true;
	anim_timer = initial_time;
}

void Burger_Stack::update(){
	update_falling_ingred();
	if(!in_anim) return;
	anim_timer += GetFrameTime();

	if(anim_timer >= 1){
		anim_timer = 1;
		in_anim = false;
	}
	if(number_ingred != 0 && anim_timer > 0.5f) clear();
	burger_middle_x = 48 + 400 * anim_timer * (anim_timer - 1);
	
}

void Burger_Stack::draw(){	
	// Desenha prato
	DrawEllipse(burger_middle_x, burger_bottom_y, ingred_width, ingred_width * 0.5, P_PURPLE_DARK);

	// Pao de baixo
	DrawTexture(bottom_bread, burger_middle_x  - INGRED_VIS_W * 0.5f, burger_bottom_y - INGRED_VIS_H + 7, WHITE);

	Node* offscreen_node = top;
	int offscreen_pos = 0;
	while(offscreen_node != nullptr && offscreen_node->height > burger_bottom_y){
		offscreen_node = offscreen_node->next;
		offscreen_pos++;
	}
	// Aux agora aponta pro primeiro ingrediente visivel

	int current_ingred_height;
	Ingredient cur_ingred;
	for(int pos = number_ingred - 1; pos >= offscreen_pos; pos--){
		Node* cur_node = offscreen_node;

		for(int i = 0; i < pos - offscreen_pos; i++){
			cur_node = cur_node->next;
		}

		draw_ingredient(cur_node->height, cur_node->ingred);

		if(cur_node->height > burger_bottom_y) { // Fora da tela
			break;
		}
	}
}

void draw_ingredient(int height, Burger_Stack::Ingredient ingred){
	Rectangle source = (Rectangle){(float) INGRED_VIS_W * (int)ingred, 0, INGRED_VIS_W, INGRED_VIS_H};
	Vector2 pos = (Vector2){burger_middle_x - INGRED_VIS_W * 0.5f, (float)burger_bottom_y - (INGRED_VIS_H + height)};

	DrawTextureRec(ingred_sprites, source, pos, WHITE);
}

void draw_ingredient_at(int ingred_id, int x_pos, int y_pos, float fade){
	Rectangle source = (Rectangle){(float) INGRED_VIS_W * ingred_id, 0, INGRED_VIS_W, INGRED_VIS_H};
	Vector2 pos = (Vector2){(float)x_pos, (float)y_pos};

	Color fade_color = (Color){255, (unsigned char)(255 * fade), 255, (unsigned char)(255 * fade)};
	DrawTextureRec(ingred_sprites, source, pos, fade_color);
}

void ingred_properties(int ingred_id, Color *color, int *ingred_vis_h){  // TODO: deletar isso
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


int Burger_Stack::ingredient_price(Burger_Stack::Ingredient ingred) const{
	switch(ingred){
		case Burger_Stack::Ingredient::q: return 80;
		case Burger_Stack::Ingredient::c: return 90;
		case Burger_Stack::Ingredient::s: return 85;
		case Burger_Stack::Ingredient::z: return 155; 
		case Burger_Stack::Ingredient::k: return 100;
		case Burger_Stack::Ingredient::m: return 105; 
		case Burger_Stack::Ingredient::p: return 60;
		case Burger_Stack::Ingredient::a: return 75; 
		case Burger_Stack::Ingredient::t: return 100;
		case Burger_Stack::Ingredient::n: return 375; 
		case Burger_Stack::Ingredient::o: return 100; 
		case Burger_Stack::Ingredient::b: return 350;
		default: return ERROR_CODE; // Error code
	}
}


Burger_Stack::Ingredient number_to_ingred(int num){
	switch(num){
		case  0: return Burger_Stack::Ingredient::q;
		case  1: return Burger_Stack::Ingredient::c;
		case  2: return Burger_Stack::Ingredient::s;
		case  3: return Burger_Stack::Ingredient::z;
		case  4: return Burger_Stack::Ingredient::k;
		case  5: return Burger_Stack::Ingredient::m;
		case  6: return Burger_Stack::Ingredient::p;
		case  7: return Burger_Stack::Ingredient::a;
		case  8: return Burger_Stack::Ingredient::t;
		case  9: return Burger_Stack::Ingredient::n;
		case 10: return Burger_Stack::Ingredient::o;
		case 11: return Burger_Stack::Ingredient::b;

		default: return Burger_Stack::Ingredient::OFF_LIMITS;
	}
}

Burger_Stack::Ingredient char_to_ingred(int cha){
		switch(cha){
		case 'q': return Burger_Stack::Ingredient::q;
		case 'c': return Burger_Stack::Ingredient::c;
		case 's': return Burger_Stack::Ingredient::s;
		case 'z': return Burger_Stack::Ingredient::z;
		case 'k': return Burger_Stack::Ingredient::k;
		case 'm': return Burger_Stack::Ingredient::m;
		case 'p': return Burger_Stack::Ingredient::p;
		case 'a': return Burger_Stack::Ingredient::a;
		case 't': return Burger_Stack::Ingredient::t;
		case 'n': return Burger_Stack::Ingredient::n;
		case 'o': return Burger_Stack::Ingredient::o;
		case 'b': return Burger_Stack::Ingredient::b;

		default: return Burger_Stack::Ingredient::OFF_LIMITS;
	}
}

std::string ingred_to_name(Burger_Stack::Ingredient ingred, bool first){ // espaços servem para alinhar fonte à matriz de pixel
	switch(ingred){
		case Burger_Stack::Ingredient::q: return first? "Asa de"      : " morcego";
		case Burger_Stack::Ingredient::c: return first? "Olho de"     : "  aranha";
		case Burger_Stack::Ingredient::s: return first? " Gosma de"   : "slime";

		case Burger_Stack::Ingredient::z: return first? ""            : "Pão";
		case Burger_Stack::Ingredient::k: return first? " Sangue de"  : " cthulu";
		case Burger_Stack::Ingredient::m: return first? "  Lágrimas de": "sereia";
		case Burger_Stack::Ingredient::p: return first? " Dente de"   : " alho";
		case Burger_Stack::Ingredient::a: return first? "Molho de"    : "  lava";
		case Burger_Stack::Ingredient::t: return first? " Cogumelo do": " pântano";
		
		case Burger_Stack::Ingredient::n: return first? " Carne de"   : "    serpente";
		case Burger_Stack::Ingredient::o: return first? " Ovo de"     : " dragão";
		case Burger_Stack::Ingredient::b: return first? " Cabelo de"  : " unicórnio";
		default: return "Ingred. desconhecido";
	}
}

void Burger_Stack::init_ingred_sprites(){
	ingred_sprites = LoadTexture("assets/sprites/ingred.png");
	bottom_bread = LoadTexture("assets/sprites/bottom_bread.png");
}

void Burger_Stack::unload_sprites(){
	UnloadTexture(ingred_sprites);
	UnloadTexture(bottom_bread);
}