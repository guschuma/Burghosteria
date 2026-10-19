#pragma once // Inclua esse header apenas uma vez
#include <chrono>

struct Timer {std::chrono::steady_clock::time_point start;};
void timer_start(Timer *timer);
double timer_end(Timer *timer); 




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