#include "log_timer.hpp"
#include <iostream> // TODO: remover isso
#include <fstream>

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

void print_log(){ // Emite log em medicoes.log
	std::ofstream medicoes_log("bin/medicoes.log");

	if(!medicoes_log.is_open()) {
		std::cout << "Falha ao escrever em medicoes.log\n\n";
		return;
	}
	debug_print_measures();
	medicoes_log << 
	"=== RELATORIO DE DESEMPENHO ==="
	<< "\n\n";

	medicoes_log << 
	"-- AVL: carregamento --"
	<< "\n\n";

	medicoes_log << 
	"Nos inseridos: \t\t\t\t" << measures_vec[AVL_INSERTED_NODES]
	<< "\n";

	medicoes_log << 
	"Altura resultante: \t\t\t" << measures_vec[AVL_RESULTING_HEIGHT]
	<< "\n";

	medicoes_log << 
	"Rotacoes (insercao): \t\t" << measures_vec[AVL_N_ROTATIONS]
	<< "\n";

	medicoes_log << 
	"Tempo de carregamento \t\t" << measures_vec[TIME_TO_LOAD_AVL] << "s"
	<< "\n\n";

	medicoes_log << 
	"-- Historico: carregamento --"
	<< "\n\n";

	medicoes_log << 
	"Entradas carregadas: \t\t" << measures_vec[LOADED_ENTRIES_HISTORY]
	<< "\n";

	medicoes_log << 
	"Tempo para carregar: \t\t" << measures_vec[TIME_TO_LOAD_HISTORY] << "s"
	<< "\n\n";

	medicoes_log << 
	"-- AVL: buscas durante a partida --"
	<< "\n\n";

	medicoes_log << 
	"Total de buscas: \t\t\t" << measures_vec[AVL_NUMBER_SEARCHES]
	<< "\n";

	medicoes_log << 
	"Total de comparacoes: \t\t" << measures_vec[AVL_NUMBER_COMPS]
	<< "\n";

	medicoes_log << 
	"Media comp. por busca: \t\t" << measures_vec[AVL_NUMBER_COMPS] / measures_vec[AVL_NUMBER_SEARCHES]
	<< "\n";

	medicoes_log << 
	"Tempo total acumulado: \t\t" << measures_vec[TOTAL_TIME_SEARCH_AVL] << "s"
	<< "\n\n";

	medicoes_log << 
	"-- Tabela hash --"
	<< "\n\n";

	medicoes_log << 
	"Tamanho (m): \t\t\t\t" << measures_vec[SIZE_TABLE_HASH]
	<< "\n";

	medicoes_log << 
	"Entradas ocupadas (n): \t\t" << measures_vec[OCCUPIED_ENTRIES_HASH]
	<< "\n";

	medicoes_log << 
	"Fator de carga (lambda): \t" <<  100.0f * measures_vec[OCCUPIED_ENTRIES_HASH]  / measures_vec[SIZE_TABLE_HASH]<< "%"
	<< "\n";

	medicoes_log << 
	"Total de hits: \t\t\t\t" <<  measures_vec[HITS_HASH]
	<< "\n";

	medicoes_log << 
	"Total de misses: \t\t\t" <<  measures_vec[MISSES_HASH]
	<< "\n";

	medicoes_log << 
	"Taxa de acerto: \t\t\t" <<  100.0f * measures_vec[HITS_HASH] / (measures_vec[HITS_HASH] + measures_vec[MISSES_HASH]) << "%"
	<< "\n";

	medicoes_log << 
	"Total de colisoes: \t\t\t" <<  measures_vec[NUM_COLLISIONS_HASH]
	<< "\n\n";

	medicoes_log << 
	"-- Ordenacao do historico (ao acessar Historico) --"
	<< "\n\n";

	medicoes_log << 
	"Tamanho: \t\t\t\t\t" <<  measures_vec[LOADED_ENTRIES_HISTORY] 
	<< "\n";

	medicoes_log << 
	"Quick sort: \t\t\t\t" <<  measures_vec[TIME_TO_QUICK_SORT] << "s"
	<< "\n";

	medicoes_log << 
	"Media de tempo de sort: \t" <<  measures_vec[TIME_TO_QUICK_SORT] / measures_vec[NUM_OF_QUICK_SORTS] << "s" 
	<< "\n\n";


	medicoes_log << "================================";

	medicoes_log.close();
}
/*
Tamanho (m): 101
Entradas ocupadas (n): 1
Fator de carga (lambda): 0.01
Total de hits: 1
Total de misses: 1
Taxa de acerto: 50.0%
Total de colisoes: 0
-- Ordenacao do historico (ao acessar Historico) --
Tamanho: 15 entradas
Merge Sort: 0.000003 s
================================
*/