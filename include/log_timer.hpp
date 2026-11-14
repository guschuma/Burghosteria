#pragma once // Inclua esse header apenas uma vez
#include <chrono>

#define N_DIFFERENT_MEASURES 17

enum measurement {TIME_TO_LOAD_HISTORY=0, //
					LOADED_ENTRIES_HISTORY=1,//
					TIME_TO_QUICK_SORT=2,//
					SIZE_OF_QUICK_SORT=3,//
					AVL_INSERTED_NODES=4, //
					AVL_RESULTING_HEIGHT=5,
					AVL_N_ROTATIONS=6, // rotacoes singulares
					TIME_TO_LOAD_AVL=7, //
					AVL_NUMBER_SEARCHES=8, //
					AVL_NUMBER_COMPS=9,		//
					TOTAL_TIME_SEARCH_AVL=10, //
					SIZE_TABLE_HASH=11,		//
					OCCUPIED_ENTRIES_HASH=12,//
					HITS_HASH=13,			//
					MISSES_HASH=14,
					NUM_COLLISIONS_HASH=15,	//
					NUM_OF_QUICK_SORTS=16, //
					};
struct Timer {std::chrono::steady_clock::time_point start;};
void timer_start(Timer *timer);
double timer_end(Timer *timer, measurement measure); 
double get_measure(measurement measure);
void set_measure(measurement measure, double value);
void add_one_to_measure(measurement measure);

void debug_print_measures();

void print_log();
