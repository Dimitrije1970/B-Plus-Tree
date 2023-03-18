#include <queue>
#include <stack>
#include "b_plus_tree.h"

BPlusTree::BPlusTree(int m) : m(m), root(nullptr), height(-1) {};


BPlusTree::~BPlusTree() {
	std::queue<Node*> q;

	q.push(root);
	while (q.empty() != true) {
		Node* temp = q.front();
		q.pop();

		for (int i = 0; i <= temp->num; i++) {
			if(!temp->leaf) q.push(temp->pointers[i]);
		}
		
		if(temp) delete temp;
	}
}

void BPlusTree::insert_data(ll key, ll ca_b_id, ll ca_c_id, ll ca_tax_st, std::string ca_name, double ca_bal) {
	Node::Data* new_data = new Node::Data(ca_b_id, ca_c_id, ca_tax_st, ca_name, ca_bal);

	if (root == NULL) {
		root = new Node(m, true);
		root->keys[0] = key;
		root->data[0] = new_data;
		root->num = 1;
		height = 0;
	}
	else {
		Node* current = root;
		Node* parent = nullptr;
		while (current->leaf == false) {
			parent = current;
			for (int i = 0; i < current->num; i++) {
				if (key < current->keys[i]) {
					current = current->pointers[i];
					break;
				}
				if (i == current->num - 1) {
					current = current->pointers[i + 1];
					break;
				}
			}
		}
		if (current->num < m) {
			int i = 0;
			while (key > current->keys[i] && i < current->num)
				i++;
			for (int j = current->num; j > i; j--) {
				current->keys[j] = current->keys[j - 1];
				current->data[j] = current->data[j - 1];
			}
			current->keys[i] = key;
			current->data[i] = new_data;
			current->num++;
			current->pointers[current->num] = current->pointers[current->num - 1];
			current->pointers[current->num - 1] = NULL;
		}
		else {
			Node* new_leaf = new Node(m, true);
			ll* virtual_node = new ll[m+1];
			Node::Data** virtual_data = new Node::Data * [m + 1];

			for (int i = 0; i < m; i++) {
				virtual_node[i] = current->keys[i];
				virtual_data[i] = current->data[i];
			}

			int i = 0, j;

			while (key > virtual_node[i] && i < m)
				i++;

			for (int j = m + 1; j > i; j--) {
				virtual_node[j] = virtual_node[j - 1];
				virtual_data[j] = virtual_data[j - 1];
			}

			virtual_node[i] = key;
			virtual_data[i] = new_data;

			new_leaf->leaf = true;
			current->num = (m + 1) / 2;
			new_leaf->num = m + 1 - (m + 1) / 2;
			current->pointers[current->num] = new_leaf;
			new_leaf->pointers[new_leaf->num] = current->pointers[m];
			current->pointers[m] = NULL;

			for (i = 0; i < current->m; i++) {
				current->keys[i] = virtual_node[i];
				current->data[i] = virtual_data[i];
			}

			for (i = 0, j = current->num; i < new_leaf->num; i++, j++) {
				new_leaf->keys[i] = virtual_node[j];
				new_leaf->data[i] = virtual_data[j];
			}

			if (current == root) {
				Node* new_root = new Node(m, false);

				new_root->keys[0] = current->keys[current->num - 1];
				new_root->pointers[0] = current;
				new_root->pointers[1] = new_leaf;
				new_root->leaf = false;
				new_root->num = 1;
				root = new_root;

				height++;
			}
			else {
				insert_internal(current->keys[current->num - 1], parent, new_leaf);
			}
		}
	}
}

void BPlusTree::insert_internal(ll key, Node* current, Node* child) {
	if (current->num < m) {
		int i = 0;
		while (key > current->keys[i] && i < current->num)
			i++;
		for (int j = current->num; j > i; j--) {
			current->keys[j] = current->keys[j - 1];
		}
		for (int j = current->num + 1; j > i + 1; j--) {
			current->pointers[j] = current->pointers[j - 1];
		}
		current->keys[i] = key;
		current->num++;
		current->pointers[i + 1] = child;
	}
	else {
		Node* new_internal = new Node(m, false);
		ll* virtual_key = new ll[m+1];
		Node** virtual_pointer = new Node * [m + 2];

		for (int i = 0; i < m; i++) {
			virtual_key[i] = current->keys[i];
		}

		for (int i = 0; i < m + 1; i++) {
			virtual_pointer[i] = current->pointers[i];
		}

		int i = 0, j;
		while (key > virtual_key[i] && i < m)
			i++;

		for (int j = m + 1; j > i; j--) {
			virtual_key[j] = virtual_key[j - 1];
		}

		virtual_key[i] = key;

		for (int j = m + 2; j > i + 1; j--) {
			virtual_pointer[j] = virtual_pointer[j - 1];
		}

		virtual_pointer[i + 1] = child;
		new_internal->leaf = false;
		current->num = (m + 1) / 2;
		new_internal->num = m - (m + 1) / 2;

		for (i = 0, j = current->num + 1; i < new_internal->num; i++, j++) {
			new_internal->keys[i] = virtual_key[j];
		}

		for (i = 0, j = current->num + 1; i < new_internal->num + 1; i++, j++) {
			new_internal->pointers[i] = virtual_pointer[j];
		}

		if (current == root) {
			Node* new_root = new Node(m, false);
			new_root->keys[0] = current->keys[current->num];
			new_root->pointers[0] = current;
			new_root->pointers[1] = new_internal;
			new_root->leaf = false;
			new_root->num = 1;
			root = new_root;
		}
		else {
			insert_internal(current->keys[current->num], find_parent(root, current), new_internal);
		}
	}
}

Node* BPlusTree::find_parent(Node* cursor, Node* child) {
	Node* parent = nullptr;
	if (cursor->leaf || (cursor->pointers[0])->leaf) {
		return NULL;
	}
	for (int i = 0; i < cursor->num + 1; i++) {
		if (cursor->pointers[i] == child) {
			parent = cursor;
			return parent;
		}
		else {
			parent = find_parent(cursor->pointers[i], child);
			if (parent != NULL)
				return parent;
		}
	}
	return parent;
}

void BPlusTree::print_tree(Node* cursor) const {
	std::queue<Node*> q;

	int cnt = 0;

	q.push(root);
	q.push(nullptr);
	while (!q.empty()) {
		Node* temp = q.front();
		q.pop();

		if (temp) {
			if (!temp->leaf) {
				for (int i = 0; i <= temp->num; i++) {
					q.push(temp->pointers[i]);
				}
			}
			temp->print_node(); std::cout << "   ";
			cnt = 0;
		}
		else {
			std::cout << std::endl;
			q.push(nullptr);
			cnt++;
			if (cnt == 2) break;
		}
	}

}

void BPlusTree::delete_data(ll key) {
	std::cout << "Unesi kljuc za pretragu: ";  std::cin >> key;

	Node* lowest = this->get_root();
	BPlusTree* bpt_n = new BPlusTree(m);

	while (lowest->get_pointers()[0]) {
		lowest = lowest->get_pointers()[0];
	}

	while (lowest) {
		for (int i = 0; i < lowest->get_num(); i++) {
			if (lowest->get_keys()[i] != key) {
				bpt_n->insert_data(lowest->get_keys()[i], lowest->get_data()[i]->ca_b_id, lowest->get_data()[i]->ca_c_id, lowest->get_data()[i]->ca_tax_st, lowest->get_data()[i]->ca_name, lowest->get_data()[i]->ca_bal);
			}
		}

		lowest = lowest->pointers[lowest->num];
	}

	std::cout << "Zapis uspesno obrisan." << std::endl << std::endl;
}

Node* BPlusTree::search_bpt(ll key, int* num_of_steps) const {
	Node* target = root;
	int i;

	while (target->leaf != true) {
		for (i = 0; i < target->num+1; i++) {
			if (i == target->num) {
				target = target->pointers[i];
				break;
			}
			(* num_of_steps)++;
			if (key <= target->keys[i]) {
				target = target->pointers[i];
				break;
			}
		}
	
	}

	for (int i = 0; i < target->num; i++) {
		(*num_of_steps)++;
		if (target->keys[i] == key) {
			return target;
		}
	}
 
	return nullptr;
}