#include "structs/history.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <log_timer.hpp>

History::History(){
	cont_list = new Node[INITIAL_VEC_SIZE];
	max_size = INITIAL_VEC_SIZE;
	current_size = 0;
}

History::~History(){
	delete[] cont_list;
	current_size = 0;
	max_size = 0;
}

void History::insert(const char *new_name, int new_score, bool insert_in_file){ // Tambem insere no arquivo, NAO FAZ o sort
	size_t size_name = std::strlen(new_name); 
	if(size_name >= MAX_NAME_SIZE) {
		std::cout << "\nNome muito grande : " << new_name << "\n";
		return;
	}

	if(current_size == max_size) {
		// Eh necessario alocar mais memoria
		Node *new_cont_list = new Node[max_size * 2];
		
		for(int i = 0; i < current_size; i++){
			new_cont_list[i] = cont_list[i];
		}

		delete[] cont_list;
		cont_list = new_cont_list;
		max_size *= 2;
	}
	Node &new_node = cont_list[current_size];

	strcpy(new_node.name, new_name);
    new_node.score = new_score;

	current_size++;
	
	
	
	// Coloca no arquivo
	if(insert_in_file){
		std::ofstream file(history_file_name, std::ios::app);
		if(!file.is_open()){
			std::cout << "Erro ao abrir " << history_file_name << "\n\n";
			return;
		}
		file << new_node.name  << "\n";
		file << new_node.score << "\n";

		file.close();
	}

	//// insertion sort no ultimo elemento para coloca-lo em ordem
	//for (int i = current_size - 1; i > 0; i--){
	//	if(cont_list[i - 1].score <= cont_list[i].score) break;

	//	swap(i - 1, i);
	//}
}

void History::print(){
	for(int i = 0; i < current_size; i++){
		if(i % 1 == 0) std::cout << cont_list[i].name << " : " << cont_list[i].score << "\n"; // Nao printa todos os nomes
	}
}


void History::read_from_file(){
	Timer tim;
	timer_start(&tim);
	int loaded_entries;

	std::ifstream file(history_file_name);

	if(!file.is_open()){
		std::cout << "Erro ao abrir " << history_file_name << "\n\n";
		return;
	}

	std::string name;
	std::string score;

	while(std::getline(file, name) && std::getline(file, score)){
		insert(name.c_str(), stoi(score));
		loaded_entries++;
	}

	file.close();
	timer_end(&tim, TIME_TO_LOAD_HISTORY);
	set_measure(LOADED_ENTRIES_HISTORY, loaded_entries);
	std::cout << "Tempo para carregar historico: " << get_measure(TIME_TO_LOAD_HISTORY) << "\n";
	std::cout << "Entradas carregadas: " << get_measure(LOADED_ENTRIES_HISTORY) << "\n";
}

void History::swap(int a_id, int b_id){
	Node aux = cont_list[b_id];
	cont_list[b_id] = cont_list[a_id];
	cont_list[a_id] = aux;
}

void History::sort(bool in_alphabetical){
	Timer tim;
	timer_start(&tim);
	sort(0, current_size - 1, in_alphabetical);
	timer_end(&tim, TIME_TO_QUICK_SORT);
	std::cout << "Tempo para quick sort:" << get_measure(TIME_TO_QUICK_SORT) << "\n\n";
}
void History::sort(int start_id, int end_id, bool in_alphabetical){ 
	if(start_id >= end_id){
		return;
	}
	int pivot;
	if(in_alphabetical){
		pivot = partition_lexico(start_id, end_id);
	}
	else pivot = partition(start_id, end_id);

	sort(start_id, pivot - 1, in_alphabetical);
	sort(pivot + 1, end_id, in_alphabetical);
}

int History::partition(int start_id, int end_id){ // Particiona em ordem numerica de scores decrescentes
	// Adaptado de https://www.geeksforgeeks.org/c/quick-sort-in-c/
	int pivot_value = cont_list[start_id].score; 

	int low_pivot = start_id;
	int hig_pivot = end_id;

	while(low_pivot < hig_pivot) {
		// Find the first element smaller than
        // the pivot (from starting)
        while (cont_list[low_pivot].score >= pivot_value && low_pivot <= end_id - 1) {
        	low_pivot++;
        }

        // Find the first element greater than
        // the pivot (from last)
        while (cont_list[hig_pivot].score < pivot_value && hig_pivot >= start_id + 1) {
            hig_pivot--;
        }
        if (low_pivot < hig_pivot) {
            swap(low_pivot, hig_pivot);
        }
	}
	swap(start_id, hig_pivot); // Troca pivo com o elemento do meio (coloca no lugar certo)
	return hig_pivot;
}

int History::partition_lexico(int start_id, int end_id){
	char *pivot_value = cont_list[start_id].name; 

	int low_pivot = start_id;
	int hig_pivot = end_id;

	while(low_pivot < hig_pivot) {
		// Find the first element smaller than
        // the pivot (from starting) cont_list[low_pivot].score >= pivot_value
        while (low_pivot <= end_id - 1 && strcasecmp(cont_list[low_pivot].name, pivot_value) <= 0) {
        	low_pivot++;
        }

        // Find the first element greater than
        // the pivot (from last)
        while (hig_pivot >= start_id + 1 && strcasecmp(pivot_value, cont_list[hig_pivot].name) < 0) {
            hig_pivot--;
        }
        if (low_pivot < hig_pivot) {
            swap(low_pivot, hig_pivot);
        }
	}
	swap(start_id, hig_pivot); // Troca pivo com o elemento do meio (coloca no lugar certo)
	return hig_pivot;
}

char* History::get_name_at(int id){
	if(id < 0 || id >= current_size) return nullptr;
	return cont_list[id].name;
}

int History::get_score_at(int id){
	if(id < 0 || id >= current_size) return -1;

	return cont_list[id].score;
}