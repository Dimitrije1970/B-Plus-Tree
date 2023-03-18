#ifndef _b_plus_tree_h_
#define _b_plus_tree_h_
#define ll long long

#include "node.h"

class BPlusTree {
private:
	int m, height;
	Node* root;

public:
	BPlusTree(int m);
	BPlusTree(const BPlusTree& bpt) = delete;
	BPlusTree& operator=(const BPlusTree& bpt) = delete;

	Node* get_root() const { return root; }
	int get_height() const { return height; }

	void insert_data(ll key, ll ca_b_id, ll ca_c_id, ll ca_tax_st, std::string ca_name, double ca_bal);
	void insert_internal(ll, Node*, Node*);
	Node* find_parent(Node*, Node*);

	void delete_data(ll key);
	void print_tree(Node*) const;

	Node* search_bpt(ll key, int* num_of_steps) const;

	~BPlusTree();
};

#endif