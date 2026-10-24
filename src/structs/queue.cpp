#include "structs/queue.hpp"
#include "log_timer.hpp"
#include <cstdlib> // For rand
#include <iostream>
#include "global.hpp"
#include "raymath.h"

#define INITIAL_GHOST_PATIENCE 30
#define MAX_GHOST_PATIENCE 100
#define QUEUE_X_POS 103
#define QUEUE_Y_POS 3

#define GHOST_WIDTH 36		// Nao contando gap entre fantasmas, que eh 2
#define GHOST_HEIGHT 42

#define LEAVING_POS (Vector2){400, -40}
#define TIME_ADDED_GHOST_PATIENCE 25.0
// https://www.inf.ufrgs.br/~dgbalreira/ed/aula9b/index.html

Emotion Client_Queue::dequeue(){
	if(first == nullptr) {
		std::cout << "Erro com dequeue! Dequeue com fila vazia\n\n\n";
		return NEUTRAL;
	}
	Emotion first_emotion = first->emotion;
	Node *old_first = first;
	first = first->next;
	delete old_first;
	number_visible_clients--;
	number_actual_clients--;
	return first_emotion;
}

void Client_Queue::queue_rando(){ // Aumenta o numero de fantasmas visiveis
	Node *old_last = last;
	Node *new_last = new Node;
	if(old_last != nullptr)
		old_last->next = new_last;
	else first = new_last; // Fila tem so um elemento
	
	last = new_last;
	new_last->next = nullptr;

	new_last->current_pos = {0, -40};
	new_last->desired_pos = {(float)(INTERNAL_WIDTH - (number_visible_clients + 1) * (2 + GHOST_WIDTH)), QUEUE_Y_POS};

	new_last->order_id = (std::rand() % (int)get_measure(AVL_INSERTED_NODES)) + 1;
	new_last->patience = INITIAL_GHOST_PATIENCE; 
	//new_last->sprite = NORMAL;
	new_last->anim_state = NORMAL;
	new_last->emotion = NEUTRAL;

	number_visible_clients++;
}

void Client_Queue::set_number_clients_round(int number_clients) { 
	if(number_actual_clients != 0) return; 
	number_actual_clients = number_clients;

	// Queue o primeiro cliente
	queue_rando();
	first->anim_state = ENTERING;
}

void Client_Queue::print(){
	Node *aux = first;
	std::cout << "\n===================================\n";
	std::cout << "Visibles: " << number_visible_clients << " : Actual: " << number_actual_clients << "\n\n";
	while(aux != nullptr){
		std::cout << "Id: " << aux->order_id << " : Patience: " << aux->patience << " : State: " << aux->anim_state <<"\n";
		aux = aux->next;
	}
	std::cout << "\n\n";
}

Status_Queue Client_Queue::update(float second_passed){ // Second passed usado para animacao do fantasma flutuando
	Status_Queue status_queue = NOTHING;
	Node *aux = first;
	int number_queue = 0;
	
	//if(second_passed == 0.0f) {
	//	print();
	//}
	while(aux != nullptr){
		if(aux->anim_state == NORMAL) aux->desired_pos.y = QUEUE_Y_POS + 10 * sin(-number_queue + 2 * PI * second_passed);
		if(aux->emotion == ANGRY) { // Fantasma chacoalhando de raiva
			aux->current_pos.x += 30 * GetFrameTime() * cos(4 * PI * 3 * second_passed);
		}

		Vector2 ghost_velocity = Vector2Subtract(aux->desired_pos, aux->current_pos);
		ghost_velocity = Vector2Scale(ghost_velocity, GetFrameTime() * 0.75);
		ghost_velocity.y *= 3; // para que ele suba e desca mais rapidamente
		aux->current_pos = Vector2Add(aux->current_pos, ghost_velocity);

		update_if_reached_destination(aux, number_queue);
		
		Node* next_aux = aux->next;
		if(aux->current_pos.x > INTERNAL_WIDTH) { // Fantasma completamente saiu
			remove_first_ghost(aux, number_queue);
			status_queue = QUEUE_MOVED;
		}
		
		aux = next_aux;
		
		number_queue++;
		if(number_queue == 5) {
			return status_queue;
		}
	}
	if(number_queue == 0 && get_number_of_invisible_ghosts() != 0) {
		queue_rando();
	}
	return status_queue;
}

void Client_Queue::decrease_all_patience(){
	Node *aux = first;

	while(aux != nullptr){
		aux->patience--;
		if(aux->patience < 0.1 * MAX_GHOST_PATIENCE && aux->anim_state != LEAVING) {
			aux->emotion = ANGRY;
		}
		if(aux->patience == -1){
			aux->anim_state = LEAVING;
			aux->desired_pos = LEAVING_POS;
		}
		
		aux = aux->next;
	}

}

void Client_Queue::draw(){
	Node *aux = first;
	int number_queue = 0;
	while(aux != nullptr){
		Vector2 ghost_pos = aux->current_pos;

		// Desenhando fantasma
		Color ghost_color = (aux->emotion == ANGRY)? RED : GREEN;
		ghost_color = (aux->emotion == HAPPY)? YELLOW : ghost_color;
		DrawRectangleRounded((Rectangle){(float)(int)ghost_pos.x, (float)(int)ghost_pos.y, GHOST_WIDTH, GHOST_HEIGHT}, 0.8, 12, ghost_color);

		// Desenhando paciencia
		DrawRectangle(ghost_pos.x + 1, QUEUE_Y_POS + 0.95 * GHOST_WIDTH, GHOST_WIDTH - 2, 3, LIGHTGRAY);

		DrawRectangle(ghost_pos.x + 1, QUEUE_Y_POS + 0.95 * GHOST_WIDTH + 1, (1 - aux->patience / (float)MAX_GHOST_PATIENCE) * (GHOST_WIDTH - 2), 1, RED);
		aux = aux->next;
		
		number_queue++;
		if(number_queue == 5) return; // Nao desenha fantasmas alem de 5
	}
}

void Client_Queue::update_if_reached_destination(Node *aux, int number_queue){
	if(aux->anim_state == ENTERING && aux->desired_pos.x - 20 <= aux->current_pos.x) { 
		// Fantasma na frente chegou proximo ao destino
		if(aux == last && number_visible_clients < number_actual_clients) {
			queue_rando();
		}
		if(aux != last) {
			aux->next->anim_state = ENTERING;
			aux->next->desired_pos.x = (float)INTERNAL_WIDTH - (number_queue + 2) * (2 + GHOST_WIDTH);
			int *next_ghost_patience = &aux->next->patience;
			*next_ghost_patience = aux->patience + TIME_ADDED_GHOST_PATIENCE;
			if(*next_ghost_patience > MAX_GHOST_PATIENCE) *next_ghost_patience = MAX_GHOST_PATIENCE;
		}
		aux->anim_state = NORMAL;
	}
}

Emotion Client_Queue::remove_first_ghost(Node *aux, int number_queue){
	if(aux->next != nullptr){
		aux->next->anim_state = ENTERING;
		aux->next->desired_pos.x = (float)INTERNAL_WIDTH - (number_queue + 1) * (2 + GHOST_WIDTH);
	}
	
	return dequeue();
}

int Client_Queue::get_first_order_id(){
	if(first == nullptr) return -1;
	return first->order_id;
}

void Client_Queue::make_first_leave(Emotion emotion){
	first->anim_state = LEAVING;
	first->emotion = emotion;
	first->desired_pos = LEAVING_POS;
}