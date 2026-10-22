#pragma once // Inclua esse header apenas uma vez
#include <chrono>

#define N_DIFFERENT_MEASURES 16

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
					};
struct Timer {std::chrono::steady_clock::time_point start;};
void timer_start(Timer *timer);
double timer_end(Timer *timer, measurement measure); 
double get_measure(measurement measure);
void set_measure(measurement measure, double value);
void add_one_to_measure(measurement measure);

void debug_print_measures();
/*
=== RELATORIO DE DESEMPENHO ===
-- AVL: carregamento --
Nos inseridos: 3
Altura resultante: 2
Rotacoes (insercao): 1
Tempo de carregamento: 0.000012 s

-- Historico: carregamento --
Entradas carregadas: 15

-- AVL: buscas durante a partida --
Total de buscas: 1
Total de comparacoes: 2
Media de comparacoes por busca: 2.00
Tempo total acumulado: 0.000005 s

-- Tabela hash --
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