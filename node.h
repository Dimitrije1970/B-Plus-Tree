#ifndef _node_h_
#define _node_h_
#define ll long long

#include <iostream>
#include <string>;

class Node {
private:
	int m, num;
	long long* keys;
	Node** pointers;

	struct Data {
		long long ca_b_id, ca_c_id, ca_tax_st;
		std::string ca_name;
		double ca_bal;

		Data(ll ca_b_id, ll ca_c_id, ll ca_tax_st, std::string ca_name, double ca_bal)
			: ca_b_id(ca_b_id), ca_c_id(ca_c_id), ca_tax_st(ca_tax_st), ca_name(ca_name), ca_bal(ca_bal) {};
	};

	bool leaf;
	Data** data;

	friend class BPlusTree;
public:
	Node(int m, bool leaf);
	Node(const Node& node) = delete;
	Node& operator=(const Node& node) = delete;

	int get_num() const { return num; }
	long long* get_keys() const { return keys; }
	Data** get_data() const { return data; }
	Node** get_pointers() const { return pointers; };

	void print_node() const;

	~Node();
};

#endif