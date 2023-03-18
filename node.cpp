#include "node.h"

Node::Node(int m, bool leaf) : m(m), num(0), leaf(leaf), keys(new long long[m]), data(new Data* [m]), pointers(new Node* [m+1]) {
	for (int i = 0; i < m; i++) keys[i] = -1;
	for (int i = 0; i < m+1; i++) pointers[i] = nullptr;
};

Node::~Node() {
	delete[] keys;
	delete[] pointers;
	if (leaf) {
		for (int i = 0; i < num; i++) {
			delete data[i];
		}
	}
	delete[] data;

	keys = nullptr;
	data = nullptr;
	pointers = nullptr;
}

void Node::print_node() const{
	int i; std::cout << "|";
	for (i = 0; i < num; i++) std::cout << keys[i] % 10000 << "|";
	if (leaf)std::cout << "->";
}