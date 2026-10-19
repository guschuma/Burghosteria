#pragma once // Inclua esse header apenas uma vez
#include "raylib.h"

#include "structs/burger_stack.hpp"
#include "structs/food_heap.hpp"

#define BOOK_COLOR (Color){252, 205, 79, 255}
#define SELECTED_BOOK_COLOR (Color){233, 123, 33, 255}
#define CLICKED_BOOK_COLOR (Color){207, 139, 27, 255}

#define START_NUM_FOOD_STOCK 10
#define START_NUM_NON_PERISH_NOT_PREP 5

#define INITIAL_PLAYER_MONEY 1000

enum button_order {
	CHEDDAR_PREP=-3,ONION_PREP, 	SWISS_PREP,
	CHEDDAR,		ONION,			SWISS,
	BREAD,			KETCHUP,		MUSTARD,
	PICKLES,		LETTUCE,		TOMATO,
	MEAT,			EGG,			BACON,
	MEAT_PREP,		EGG_PREP,		BACON_PREP,
};

void start_gameplay();
void end_gameplay();
void update_gameplay();

void draw_gameplay();
void draw_ingredient(int height, Burger_Stack::Ingredient ingred);
void draw_burger();

void ingred_properties(int ingred_id, Color *color, int *ingred_vis_h);
void draw_ingred_being_summoned(int ingred_id, float animation_time); // Animation time is between zero and one

