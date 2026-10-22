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
	};
	Node *first = nullptr;
	Node *last = nullptr;
	int number_clients = 0;
public:
	void queue_rando();
	Emotion dequeue();
	void print();
	void update();
};