#include "structs/fx_queue.hpp"

#define SECONDS_PER_FRAME 0.05


void FX_Queue::load(){ // load textures
	summoning_t = LoadTexture("assets/sprites/fx/summoning.png");
	remove_t = LoadTexture("assets/sprites/fx/remove_fx.png");
	red_spawn_t = LoadTexture("assets/sprites/fx/red_spawn.png");
}

void FX_Queue::unload(){
	clear();
	UnloadTexture(summoning_t);
	UnloadTexture(remove_t);
	UnloadTexture(red_spawn_t);
}

void FX_Queue::update(){
	Node* aux = first;

	while(aux != nullptr){
		if(!aux->animation_ended){
			aux->time_in_current_frame += GetFrameTime();

			if(aux->time_in_current_frame >= SECONDS_PER_FRAME){
				aux->current_frame++;
				aux->time_in_current_frame = 0;

				if(aux->current_frame >= aux->max_number_of_frames){
					aux->animation_ended = true;
				}
			}
		}

		aux = aux->next;
	}

	remove_ended_fx(); // Remove os fxs que ja terminaram em sequencia
}

void FX_Queue::draw(){
	Node* aux = first;

	while(aux != nullptr){
		if(!aux->animation_ended)
			DrawTextureRec(fx_to_texture(aux->fx_type), 
									{(float)aux->width * aux->current_frame, 0, (float)aux->width, (float)aux->height},
									aux->pos, 
									WHITE);

		aux = aux->next;
	}
}

void FX_Queue::remove_ended_fx(){
	Node* aux = first;

	while(aux != nullptr){
		if(!aux->animation_ended) return;
		first = aux->next;
		delete aux;
		if(first == nullptr) {
			last = nullptr;
			aux = nullptr;
		}
		else aux = first;
	}
}
 
void FX_Queue::insert(FX new_fx, Vector2 pos){  // pos esta no meio
	Node* new_fx_node = new Node; 
	new_fx_node->height = 64;
	new_fx_node->width = 64;
	switch(new_fx){
		case summoning:
			new_fx_node->max_number_of_frames = 10;
		 	break;

		case remove_fx:
			new_fx_node->max_number_of_frames = 21;
			break;
		case red_spawn:
			new_fx_node->max_number_of_frames = 16;
			new_fx_node->current_frame = 8;
	}
	
	
	pos.x -= new_fx_node->width * 0.5;
	pos.y -= new_fx_node->height * 0.5;
	new_fx_node->pos = pos;
	new_fx_node->fx_type = new_fx;
	new_fx_node->next = nullptr;

	if(last == nullptr) {
		first = new_fx_node;
	}
	else {
		last->next = new_fx_node;
	}
	last = new_fx_node;

}

Texture2D FX_Queue::fx_to_texture(FX fx){
	switch(fx){
		case summoning: return summoning_t;
		case remove_fx: return remove_t;
		case red_spawn: return red_spawn_t;
		default: return summoning_t;
	}
}

void FX_Queue::clear(){
	Node *aux = first;

	while(aux != nullptr){
		Node *next = aux->next;
		delete aux;
		aux = next;
	}

	first = nullptr;
	last = nullptr;
}