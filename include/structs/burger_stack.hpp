#pragma once // Inclua esse header apenas uma vez

#include "global.hpp"
#include "raylib.h"
#include <string>

class Burger_Stack {
public:
	enum Ingredient{
		OFF_LIMITS = ERROR_CODE, // Error code
		q = 0, // Queijo cheddar
		c = 1, // Cebola
		s = 2, // Queijo suico
		z = 3, // Pao de cima
		k = 4, // Ketchup
		m = 5, // Mostarda
		p = 6, // Picles
		a = 7, // Alface
		t = 8, // Tomate       
		n = 9, // Carne
		o =10, // Ovo frito
		b =11, // Bacon
	};
	
	void insert_ingredient(Ingredient ingred);
	void remove_ingredient();
	void print() const;
	void clear();
	bool is_identical(const Burger_Stack &other_stack);
	void get_ingred_start_bottom(int pos, Burger_Stack::Ingredient *ingred, int *height) const;
	int get_height(){ return this->height; };
	float get_first_height() { if(top != nullptr) return top->height; return -1;};
	int get_number_ingred(){ return this->number_ingred; };
	void update_falling_ingred();
	int get_price() { return price; }
	void start_burger_leaving_anim(float initial_time);
	void update(); // Update animacao e ingredientes caindo
	bool is_in_anim() { return in_anim;}
	void draw();
	void init_ingred_sprites();
	void unload_sprites();
private:
	struct Node {
        Ingredient ingred; // Ingrediente
        Node* next = nullptr;

		float height = 0;    // Positive and relative to bottom
		int desired_height = 0;
		float velocity = 0;
		bool falling = true;
		bool invoking = true;
    };
	int price = 0;		   // Preco em centavos
	int height = 0;		   // Tamanho visual da pilha
	int number_ingred = 0; // Numero de ingredientes na pilha
	bool in_anim = false;
	float anim_timer = 0.0f; // Vai de 0 a 1
	[[nodiscard]] int ingredient_price(Ingredient ingred) const;
public:
	Node* top = nullptr;
};

void ingred_properties(int ingred_id, Color *color, int *ingred_vis_h);
void draw_ingredient(int height, Burger_Stack::Ingredient ingred);
void draw_ingredient_at(int ingred_id, int x_pos, int y_pos, float fade);

Burger_Stack::Ingredient number_to_ingred(int num);
Burger_Stack::Ingredient char_to_ingred(int cha);
char ingred_to_char(Burger_Stack::Ingredient ingred);
std::string ingred_to_string(Burger_Stack::Ingredient ing);
std::string ingred_to_name(Burger_Stack::Ingredient ingred, bool first);



extern const int ingred_tallness[12];
extern const int ingred_vis_heig[12];

extern int burger_middle_x;
extern const int burger_bottom_y;
extern const int ingred_width;
extern const int spawning_height;
extern const float ingred_falling_accel;