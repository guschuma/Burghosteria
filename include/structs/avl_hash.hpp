#pragma once

#include <string>
#include "global.hpp" // Para definicao de errorcode

#define MAX_LENGTH_RECIPE 8
#define HASH_SIZE 1000

// Baseado em https://www.inf.ufrgs.br/~dgbalreira/ed/aula14/index.html
//			  https://www.inf.ufrgs.br/~dgbalreira/ed/aula20/index.html
// Nao tem a implementao de remocao de nodo
class AVL_Hash {
private:
	struct Node{
		Node *left;
		Node *right;
		int id;
		int height; // Folha eh 0. NULL eh -1
		char recipe[MAX_LENGTH_RECIPE];
	};
	struct Node_Hash{
		int id;
		char recipe[MAX_LENGTH_RECIPE];
		Node_Hash* next;
	};
public:
	AVL_Hash();
	~AVL_Hash();

	std::string recipes_file_name;
	int avlHeight(const Node *node);
	int balanceF(const Node *node);

	Node* rebalance(Node *root_t);
	const char* search(int id_code);

	void insert(int id_code, const char *recipe);
	void read_from_file();
	void print();

	int get_hash_table_size(){ return size_hash;}
	int get_hash_num_elem() { return hash_number_elements; }
private:
	Node* root = nullptr;
	const char* search(int id_code, Node *root);
	AVL_Hash::Node* insert(int id_code, const char *recipe, Node *root);
		
	Node* LLrot(Node *y_Node);
	Node* RRrot(Node *x_Node);

	void terminateNode(Node *node);
	void print(Node* root_t);
	void updateHeight(Node *node);
	int size_tree = 0;

	Node_Hash hash_table[HASH_SIZE];
	const int size_hash = HASH_SIZE;
	int hash_number_elements = 0;

};