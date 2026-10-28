#include "structs/avl_hash.hpp"
#include "log_timer.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
int hash_function(int num){ // Nao funciona para numeros negativos
	return (num < 0)? ERROR_CODE : num % HASH_SIZE;
}

AVL_Hash::AVL_Hash(){
	for(Node_Hash &node : hash_table){
		node.id = ERROR_CODE;
		node.next = nullptr;
		strcpy(node.recipe, "Indef");
	}
	set_measure(SIZE_TABLE_HASH, HASH_SIZE);
}

int AVL_Hash::avlHeight(const Node *node){
	if(node == nullptr) return -1;
	return node->height;
}

int AVL_Hash::balanceF(const Node *node){
	if(node == nullptr) return 0;
	return avlHeight(node->left) - avlHeight(node->right);
}

void AVL_Hash::updateHeight(Node *node){
	int hLeft = avlHeight(node->left);
	int hRight = avlHeight(node->right);

	node->height = (hLeft > hRight)? hLeft : hRight;
	node->height++;
}

AVL_Hash::Node* AVL_Hash::LLrot(AVL_Hash::Node *y_Node){
	add_one_to_measure(AVL_N_ROTATIONS);
	Node *x_Node = y_Node->left;
	Node *B_Node = x_Node->right;

	x_Node->right = y_Node;
	y_Node->left = B_Node;

	updateHeight(y_Node);
	updateHeight(x_Node);

	return x_Node;
}

AVL_Hash::Node* AVL_Hash::RRrot(AVL_Hash::Node *x_Node){
	add_one_to_measure(AVL_N_ROTATIONS);
	Node *y_Node = x_Node->right;
	Node *B_Node = y_Node->left;

	y_Node->left = x_Node;
	x_Node->right = B_Node;

	updateHeight(x_Node);
	updateHeight(y_Node);

	return y_Node;
}

AVL_Hash::Node* AVL_Hash::rebalance(AVL_Hash::Node* root_t){
	updateHeight(root_t);
	int balanceFactor = balanceF(root_t);

	// LL
	if(balanceFactor == +2 && balanceF(root_t->left) >= 0) {
		return LLrot(root_t);
	}

	// RR
	if(balanceFactor == -2 && balanceF(root_t->right) <= 0) {
		return RRrot(root_t);
	}

	// LR
	if(balanceFactor == +2 && balanceF(root_t->left) < 0){
		root_t->left = RRrot(root_t->left);
		return LLrot(root_t);
	}

	// RL
	if(balanceFactor == -2 && balanceF(root_t->right) > 0){
		root_t->right = LLrot(root_t->right);
		return RRrot(root_t);
	}
	return root_t;
}

const char* AVL_Hash::search(int id_code){
	int hashed_id_code = hash_function(id_code);
	Node_Hash *hash_node = &hash_table[hashed_id_code];
	while(hash_node->id != id_code && hash_node->id != ERROR_CODE && hash_node->next != nullptr){
		hash_node = hash_node->next; // While vai parar se 	( 1 ) chegar no item desejado
		//													( 2 ) tabela hash vazia no index (1. elem.)
		//													( 3 ) chegar no fim da cadeia
	}
	if(hash_node->id == id_code) { // ( 1 )
		add_one_to_measure(HITS_HASH);
		std::cout << "1." << "\n";
		return hash_node->recipe;
	} 

	Node_Hash *new_node;
	
	if (hash_node->id == ERROR_CODE) {
		new_node = hash_node;
		add_one_to_measure(OCCUPIED_ENTRIES_HASH);
	} else {
		add_one_to_measure(NUM_COLLISIONS_HASH);
		new_node = new Node_Hash;
		hash_node->next = new_node;
	}

	new_node->next = nullptr;
	new_node->id = id_code;

	add_one_to_measure(MISSES_HASH);
	add_one_to_measure(AVL_NUMBER_SEARCHES);
	Timer tim;
	timer_start(&tim);

	strcpy(new_node->recipe, search(id_code, root));
	timer_end(&tim, TOTAL_TIME_SEARCH_AVL);
	hash_number_elements++;

	return new_node->recipe;
}

const char* AVL_Hash::search(int id_code, Node *root_t){
	add_one_to_measure(AVL_NUMBER_COMPS); // Considerei tudo isso como uma unica comparacao
	if(root_t == nullptr) return nullptr;
	if(root_t->id == id_code) return root_t->recipe;
	if(id_code < root_t->id) return search(id_code, root_t->left);
	return search(id_code, root_t->right);
}

void AVL_Hash::insert(int id_code, const char *recipe){
	size_t size_recipe = std::strlen(recipe);
	if(size_recipe >= MAX_LENGTH_RECIPE){
		std::cout << "Receita muito grande para ser adicionada!\n";
		return;
	} 
	root = insert(id_code, recipe, root);
	size_tree++;
}

AVL_Hash::Node* AVL_Hash::insert(int id_code, const char *recipe, Node *t_root){
	if(t_root == nullptr){
		Node *new_node_ptr = new Node;
		strcpy(new_node_ptr->recipe, recipe);
		new_node_ptr->id = id_code;
		new_node_ptr->left = nullptr;
		new_node_ptr->right = nullptr;
		new_node_ptr->height = 0;

		return new_node_ptr;
	}
	if(id_code < t_root->id){
		t_root->left = insert(id_code, recipe, t_root->left);
	}
	else if(id_code > t_root->id){
		t_root->right = insert(id_code, recipe, t_root->right);
	}
	else return t_root; // Duplicate value

	return rebalance(t_root);
}


void AVL_Hash::print(){
	std::cout << "========================================\n";
	this->print(root);
	std::cout << "========================================\n";
}

void AVL_Hash::print(AVL_Hash::Node* root_t){
	if(root_t == nullptr) return;
	this->print(root_t->left);
	for(int i = 0; i < avlHeight(root_t); i++){
		std::cout << "\t";
	}
	std::cout << root_t->id << " : " << root_t->recipe << "\n";
	this->print(root_t->right);
}

void AVL_Hash::read_from_file(){
	std::ifstream file(recipes_file_name);

	if(!file.is_open()){
		std::cout << "Erro ao abrir " << recipes_file_name<< "\n\n";
		return;
	}
	std::string recipe;
	std::string id_code;

	Timer tim;
	timer_start(&tim);

	while(std::getline(file, id_code) && std::getline(file, recipe)){
		insert(stoi(id_code), recipe.c_str());
	}
	file.close();
	timer_end(&tim, TIME_TO_LOAD_AVL);
	set_measure(AVL_INSERTED_NODES, size_tree);
	set_measure(AVL_RESULTING_HEIGHT, avlHeight(root));
}

AVL_Hash::~AVL_Hash(){
	terminateNode(root);
	Node_Hash* next;
	for(int index = 0; index < HASH_SIZE; index++){
		Node_Hash* node_cur = hash_table[index].next;
		while(node_cur != nullptr){
			next = node_cur->next;
			delete node_cur;
			node_cur = next;
		}
	}
}

void AVL_Hash::terminateNode(AVL_Hash::Node *node){
	if(node == nullptr) return;

	terminateNode(node->left);
	terminateNode(node->right);

	delete node;
}
