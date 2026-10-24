#pragma once

#include "raylib.h"

enum Ghost_Sprites {
	a1, a2, a3,
};

enum Animation_State{
	NORMAL, 
	ENTERING, // Indo um espaco para a esquerda
	LEAVING,  // Apos receber o pedido
};

enum Emotion{
	NEUTRAL, 
	ANGRY,  // Indo um espaco para a esquerda
	HAPPY,  // Apos receber o pedido
};

enum Status_Queue{
	NOTHING,
	QUEUE_MOVED,
};

class Client_Queue{
private:
	struct Node {
		Node *next;
		Vector2 current_pos;
		Vector2 desired_pos;
		int order_id;
		Ghost_Sprites sprite;
		Animation_State anim_state;
		Emotion emotion;
		int patience;
	};
	Node *first = nullptr;
	Node *last = nullptr;
	int number_visible_clients = 0;
	int number_actual_clients = 0;
public:
	void queue_rando();
	Emotion dequeue();
	Emotion remove_first_ghost(Node *aux, int number_queue);
	void print();
	Status_Queue update(float second_passed);
	void draw();
	void decrease_all_patience();

	void update_if_reached_destination(Node *aux, int number_queue);

	void set_number_clients_round(int number_clients);
	int get_first_order_id();

	void make_first_leave(Emotion emotion);

	int get_number_of_invisible_ghosts() { return number_actual_clients - number_visible_clients + (int)(number_visible_clients > 5);} // number_visible_clients inclui fantasma invisivel, ironicamente
};