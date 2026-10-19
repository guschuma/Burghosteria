#include "log_timer.hpp"

void timer_start(Timer *timer){
	timer->start = std::chrono::steady_clock::now();
}

double timer_stop(Timer *timer){
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration<double>(end - timer->start).count();
}