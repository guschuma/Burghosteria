#pragma once
// historico eh um vetor contiguo
#include "log_timer.hpp"
#include <string>

#define MAX_NAME_SIZE 32
// 3125 = 200000/64
#define INITIAL_VEC_SIZE 4096

class History {
public:
	History();
	~History();
	void insert(const char new_name[], int new_score, bool insert_in_file = false);
	void print();
	void read_from_file();
	void sort();
	std::string history_file_name;
private:
	struct Node {
		char name[MAX_NAME_SIZE]; // ponteiro para vetor de tamanho max 32
		int score;
	};
	int partition(int start_id, int end_id); // Particiona e retorna id do pivo
	void swap(int a_id, int b_id);
	void sort(int start_id, int end_id);
	Node *cont_list;
	int current_size = 0;
	int max_size;
};