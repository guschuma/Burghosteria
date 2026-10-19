#include "structs/burger_stack.hpp"
#include <iostream>
#include "log_timer.hpp"
#include "global.hpp"
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

const int ingred_tallness[12] = {10,10,10,15,12,17,12,13,11,12,11,10};
const int ingred_vis_heig[12] = {13,10,14,17,13,17,14,13,15,15,12,11};

      int burger_middle_x = 48;
const int burger_bottom_y = 160;
const int ingred_width = 40;
const int spawning_height = 70;
const float ingred_falling_accel = 4.0f;

void Burger_Stack::insert_ingredient(Burger_Stack::Ingredient ingred){
	if(ingred == Ingredient::OFF_LIMITS) return;
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

std::string string_ingredient(Burger_Stack::Ingredient ing){
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
		std::cout << string_ingredient(aux->ingred) << " \t- Alt: " << aux->height
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