#pragma once

#include "raylib.h"

enum FX {
	summoning,
	remove_fx,
	red_spawn,
	correct,
	wrong,
};

class FX_Queue{
private:
	struct Node {
		Node *next;
		FX fx_type;
		float time_in_current_frame = 0.0f;
		int current_frame = 0;
		int max_number_of_frames;

		int height;
		int width;

		Vector2 pos;

		bool animation_ended = false;
	};
	Node *first = nullptr;
	Node *last = nullptr;

	Texture2D summoning_t;
	Texture2D remove_t;
	Texture2D red_spawn_t;
	Texture2D correct_t;
	Texture2D wrong_t;
public:
	void load();
	void unload();
	void print();
	void update();
	void draw();
	void insert(FX new_fx, Vector2 pos);
	void delete_ended_anims();

	void remove_ended_fx();
	Texture2D fx_to_texture(FX fx);

	void clear();
};