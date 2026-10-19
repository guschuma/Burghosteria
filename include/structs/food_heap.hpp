#pragma once // Inclua esse header apenas uma vez

#define INITIAL_HEAP_SIZE 1

class Food_Heap {
public:
	void initialize();
	void insert(int new_time_till_expire);
	void print_heap();
	void heapify(int node_id);
	void remove_id(int node_id);
	int get_root_value();
	void terminate();
	int get_current_size();
	void decrease_by_one();
	bool is_empty(){ return current_size == 0;}
	int get_id_value(int idh);
private:
	int* heap = nullptr; // Raiz, elemento mais pequeno, por ser heap. Ponteiro para o começo do vetor. Tempo até a comida vencer
	int max_size = 0;
	int current_size = 0;
};