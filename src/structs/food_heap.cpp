#include "structs/food_heap.hpp"
#include <iostream>
#include "global.hpp"
void Food_Heap::initialize(){
	this->max_size = INITIAL_HEAP_SIZE;
	this->current_size = 0;

	this->heap = new int[INITIAL_HEAP_SIZE];
}

void Food_Heap::insert(int new_time_till_expire){
	if(this->current_size == this->max_size){
		int *new_heap = new int[2 * this->max_size];
		for(int id = 0; id < this->max_size; id++){
			new_heap[id] = this->heap[id];
		}
		delete[] this->heap;
		this->heap = new_heap;
		this->max_size *= 2;
	}

	this->heap[current_size] = new_time_till_expire;
	int current_id = current_size;

	while(current_id > 0) {
		int father_id = (current_id-1)/2;
		if(this->heap[current_id] < this->heap[father_id]){
			int tmp = this->heap[current_id];
			this->heap[current_id] = this->heap[father_id];
			this->heap[father_id] = tmp;
			current_id = father_id;
		}
		else break;
	}	
	this->current_size++;

	this->print_heap();
}

bool is_power_of_two(int n){
	// so para calcular resto de log2 rapidamente
	while(n > 1){
		if(n % 2 == 1) return false;
		n /= 2;
	}
	return true;
}
void Food_Heap::print_heap(){
	std::cout << "\nMax size: " << this->max_size << ", Current size: " << this->current_size <<"\n";
	for(int id = 0; id < this->current_size; id++){
		std::cout << this->heap[id] << " ";

		if(is_power_of_two(id + 2)){
			std::cout << "\n";
		}
	}
	std::cout << "\n=================================";
}

void Food_Heap::terminate(){
	delete[] this->heap;
	this->max_size = 0;
	this->current_size = 0;
}

// Heapifies
void Food_Heap::heapify(int node_id){
	int left_id = 2 * node_id + 1;
	int right_id = 2 * node_id + 2;

	int smallest_element_id = node_id;
	if(left_id  < this->current_size && this->heap[left_id]  < this->heap[smallest_element_id]) smallest_element_id = left_id;
	if(right_id < this->current_size && this->heap[right_id] < this->heap[smallest_element_id]) smallest_element_id = right_id;

	if(smallest_element_id != node_id){
		int tmp = this->heap[smallest_element_id];
		this->heap[smallest_element_id] = this->heap[node_id];
		this->heap[node_id] = tmp;
		heapify(smallest_element_id);
	}

}

// Nao remove a raiz! EMPTY_HEAP (-10) Se heap vazio
int Food_Heap::get_root_value(){
	if(current_size == 0) return EMPTY_HEAP;
	return this->heap[0];
}

void Food_Heap::remove_id(int node_id){
	if(this->current_size == 0) return;
	
	this->current_size--;
	this->heap[node_id] = this->heap[this->current_size];

	this->heapify(node_id);
}

int Food_Heap::get_current_size(){
	return this->current_size;
}

// Decresce todos os elementos por 1 e retira negativos
void Food_Heap::decrease_by_one(){
	if(this->current_size == 0) return;
	for(int id = 0; id < this->current_size; id++){
		this->heap[id]--;
	}

	while(this->heap[0] < 0 && current_size > 0){
		this->remove_id(0);
	}
}

int Food_Heap::get_id_value(int idh){
	if(idh < 0 || idh >= current_size){
		return EMPTY_HEAP;
	}
	return this->heap[idh];
}