#include "structs/queue.hpp"
#include "log_timer.hpp"
#include <cstdlib> // For rand
#include <iostream>

// https://www.inf.ufrgs.br/~dgbalreira/ed/aula9b/index.html

Emotion Client_Queue::dequeue(){
	if(first == nullptr) {
		std::cout << "Erro com dequeue!\n\n\n";
		return NEUTRAL;
	}
	Emotion first_emotion = first->emotion;
	Node *old_first = first;
	first = first->next;
	delete old_first;
	number_clients--;
	return first_emotion;
}

void Client_Queue::queue_rando(){
	Node *old_last = last;
	Node *new_last = new Node;
	if(old_last != nullptr)
		old_last->next = new_last;
	else first = new_last; // Fila tem so um elemento
	
	last = new_last;
	new_last->next = nullptr;

	new_last->current_pos = {0, 0}; // TODO
	new_last->desired_pos = {0, 0};
	new_last->order_id = (std::rand() % (int)get_measure(AVL_INSERTED_NODES)) + 1;
	//new_last->sprite = NORMAL;
	new_last->anim_state = NORMAL;
	new_last->emotion = NEUTRAL;


	number_clients++;
}

void Client_Queue::print(){
	Node *aux = first;
	std::cout << "\n===================================\n";
	while(aux != nullptr){
		std::cout << "Id: " << aux->order_id << "\n";
		aux = aux->next;
	}
	std::cout << "\n\n";
}

void Client_Queue::update(){
	int abc = 1;
	abc++; // TODO
}