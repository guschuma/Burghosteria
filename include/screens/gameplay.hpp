#pragma once // Inclua esse header apenas uma vez
#include "raylib.h"

#include "structs/burger_stack.hpp"
#include "structs/food_heap.hpp"
#include "structs/avl_hash.hpp"
#include "global.hpp"
#define BOOK_COLOR TRANSPARENT
#define SELECTED_BOOK_COLOR (Color){233, 123, 33, 50}
#define CLICKED_BOOK_COLOR (Color){157, 99, 0, 100}

#define START_NUM_FOOD_STOCK 5
#define START_NUM_NON_PERISH_NOT_PREP 5

#define INITIAL_PLAYER_MONEY 1000

#define SHOP_TIME 45

enum button_order {
	CHEDDAR_PREP=-3,ONION_PREP, 	SWISS_PREP,
	CHEDDAR,		ONION,			SWISS,
	BREAD,			KETCHUP,		MUSTARD,
	PICKLES,		LETTUCE,		TOMATO,
	MEAT,			EGG,			BACON,
	MEAT_PREP,		EGG_PREP,		BACON_PREP,
};

extern int player_money_cents;

extern int number_non_perish[3];
extern Food_Heap meat_heap;
extern Food_Heap eggs_heap;
extern Food_Heap baco_heap;

extern AVL_Hash avl;

void init_gameplay();
void end_gameplay();
void update_gameplay();

void draw_gameplay();

void draw_burger();


void draw_ingred_being_summoned(int ingred_id, float animation_time); // Animation time is between zero and one

void spell_matrix_update();


void init_sprites_gameplay();
void end_sprites_gameplay();
