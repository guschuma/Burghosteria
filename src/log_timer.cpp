#include "log_timer.hpp"
#include <iostream> // TODO: remover isso

double measures_vec[N_DIFFERENT_MEASURES] = {0};
double time_to_load_history = 0.0f;
double time_to_quick_sort = 0.0f;
int loaded_entries_history = 0;
int size_quick_sort = 0;
void timer_start(Timer *timer){
	timer->start = std::chrono::steady_clock::now();
}

double timer_end(Timer *timer, measurement measure){
	auto end = std::chrono::steady_clock::now();
	double time_passed = std::chrono::duration<double>(end - timer->start).count();
	measures_vec[measure] += time_passed;
	return time_passed;
}

double get_measure(measurement measure){
	return measures_vec[measure];
}

void set_measure(measurement measure, double value){
	measures_vec[measure] = value;
}

void add_one_to_measure(measurement measure){
	measures_vec[measure]++;
}

void debug_print_measures(){
	std::cout << "\n==========================================\n\n";
	for(double dbl : measures_vec){
		std::cout << dbl << "\n";
	}
}